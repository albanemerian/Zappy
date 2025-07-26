folder('Branches') {
    description('Folder for Zappy related jobs branches')
}

freeStyleJob('Pull Branch from Zappy') {
    description('This job pulls a branch from the Zappipicaca repository and builds a new job to apply CI.')
    parameters {
        stringParam('BRANCH_NAME', 'main', 'GitHub branch name to pull')
    }
    steps {
        dsl {
            text('''
            folder("Branches/\${BRANCH_NAME}") {
                description("Jobs for branch \${BRANCH_NAME}")
            }

            def scmConfig = {
                git {
                    remote {
                        url("git@github.com:EpitechPromo2028/B-YEP-400-NAN-4-1-zappy-albane.merian.git")
                        credentials("my-ssh-key")
                    }
                    branches("*/\${BRANCH_NAME}")
                }
            }

            def commonProperties = {
                properties {
                    githubProjectUrl("git@github.com:EpitechPromo2028/B-YEP-400-NAN-4-1-zappy-albane.merian.git")
                }
                wrappers {
                    preBuildCleanup()
                }
            }

            // 1. Coding Style Check Job
            job("Branches/\${BRANCH_NAME}/1-Coding-Style-Check") {
                commonProperties.delegate = delegate
                commonProperties()

                scm {
                    scmConfig.delegate = delegate
                    scmConfig()
                }

                steps {
                    shell('echo "\\n\\n==== CHECKING C CODING STYLE ====\\n"')
                    shell('chmod +x /usr/local/bin/cStyleChecker.sh')
                    shell('/usr/local/bin/cStyleChecker.sh')

                    shell('echo "\\n\\n==== CHECKING C++ CODING STYLE ====\\n"')
                    shell('chmod +x /usr/local/bin/cppStyleChecker.sh')
                    shell('/usr/local/bin/cppStyleChecker.sh')

                    shell('echo "\\n\\n==== CHECKING PYTHON CODING STYLE ====\\n"')
                    shell('chmod +x /usr/local/bin/pythonStyleChecker.sh')
                    shell('/usr/local/bin/pythonStyleChecker.sh')
                }

                publishers {
                    downstream("Branches/\${BRANCH_NAME}/2-Build", 'SUCCESS')
                }

                triggers {
                    scm('* * * * *')
                }
            }

            // 2. Build Job
            job("Branches/\${BRANCH_NAME}/2-Build") {
                commonProperties.delegate = delegate
                commonProperties()

                scm {
                    scmConfig.delegate = delegate
                    scmConfig()
                }

                steps {
                    shell('echo "\\n\\n==== BUILDING SERVER ====\\n" && make zappy_server')
                    shell('echo "\\n\\n==== BUILDING GUI ====\\n" && make zappy_gui')
                    shell('echo "\\n\\n==== BUILDING AI ====\\n" && make zappy_ai')
                    shell('echo "\\n\\n==== CLEANING PROJECT ====\\n" && make clean')
                    shell('echo "\\n\\n==== DEEP CLEANING PROJECT ====\\n" && make fclean')
                    shell('echo "\\n\\n==== REBUILDING PROJECT ====\\n" && make re')
                }

                publishers {
                    downstream("Branches/\${BRANCH_NAME}/3-Tests", 'SUCCESS')
                }
            }

            // 3. Tests Job
            job("Branches/\${BRANCH_NAME}/3-Tests") {
                commonProperties.delegate = delegate
                commonProperties()

                scm {
                    scmConfig.delegate = delegate
                    scmConfig()
                }

                steps {
                    shell('echo "\\n\\n==== RUNNING UNIT TESTS WITH COVERAGE ====\\n"')
                    shell('make tests_run')
                }

                publishers {
                    archiveArtifacts {
                        pattern('coverage/**')
                        allowEmpty(true)
                    }
                }
            }
            // 4. Tests Job
            job("Branches/\${BRANCH_NAME}/4-Tests functional") {
                commonProperties.delegate = delegate
                commonProperties()

                scm {
                    scmConfig.delegate = delegate
                    scmConfig()
                }

                steps {
                    shell('echo "\\n\\n==== RUNNING UNIT TESTS WITH COVERAGE ====\\n"')
                    shell('make functional_tests')
                }

                publishers {
                    archiveArtifacts {
                        pattern('coverage/**')
                        allowEmpty(true)
                    }
                }
            }
            '''.stripIndent())
        }
    }
    triggers {
    }
}

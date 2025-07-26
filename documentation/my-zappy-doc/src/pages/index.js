import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';

export default function Home() {
  return (
    <Layout
      title="Welcome to the Zappy Documentation"
      description="Explore the documentation for the Zappy project">
      <main style={{ textAlign: 'center', padding: '2rem' }}>
        <h1>Welcome to the Zappy Documentation</h1>
        <p>Explore the different sections of the documentation:</p>
        <div style={{ marginTop: '2rem' }}>
          <Link to="/docs/Server" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            💽 Server
          </Link>
          <Link to="/docs/AI" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🧠 AI
          </Link>
          <Link to="/docs/GUI" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🎨 GUI
          </Link>
          <Link to="/docs/archi" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🏗️ Architecture
          </Link>
          <Link to="/docs/userguide" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🧭 User guide
          </Link>
          <Link to="/docs/NetworkProtocol" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            🌐 Network Protocol
          </Link>
          <Link to="docs/intro" style={{ margin: '1rem', fontSize: '1.2rem' }}>
            👨‍💻 Developer guide
          </Link>
        </div>
        <div style={{ marginTop: '3rem' }}>
          <a
            href="/DocZappy.pdf"
            target="_blank"
            rel="noopener noreferrer"
            style={{ fontSize: '1.1rem', color: '#0078e7', textDecoration: 'underline' }}
          >
            📚 View Full Doxygen PDF Documentation
          </a>
        </div>
      </main>
    </Layout>
  );
}

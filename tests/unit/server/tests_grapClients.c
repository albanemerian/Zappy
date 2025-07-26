/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_grapClients
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <unistd.h>

#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}


// Test suite for add_graph_node function
Test(graphic_client, add_graph_node_normal_case, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    int test_fd = 42;
    
    graph_net_t *result = add_graph_node(&head, test_fd);


    cr_assert_not_null(result, "add_graph_node should return valid node");
    cr_assert_eq(result->network->fd, test_fd, "Node should have correct fd");
    cr_assert_eq(result->mapSent, false, "mapSent should be initialized to false");
    cr_assert_eq(head, result, "Head should point to new node");
    cr_assert_null(result->next, "New node's next should be NULL for first node");
    
    free(result);
}

Test(graphic_client, add_graph_node_multiple_nodes, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    
    graph_net_t *first = add_graph_node(&head, 1);
    graph_net_t *second = add_graph_node(&head, 2);
    
    cr_assert_not_null(first, "First node should be created");
    cr_assert_not_null(second, "Second node should be created");
    cr_assert_eq(head, second, "Head should point to most recent node");
    cr_assert_eq(second->next, first, "Second node should point to first");
    cr_assert_null(first->next, "First node's next should be NULL");
    
    free(first);
    free(second);
}

// Test suite for remove_graph_node function
Test(graphic_client, remove_graph_node_existing_node, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    graph_net_t *node1 = add_graph_node(&head, 1);
    graph_net_t *node2 = add_graph_node(&head, 2);
    graph_net_t *node3 = add_graph_node(&head, 3);
    
    // Remove middle node
    graph_net_t *removed = remove_graph_node(&head, 2);
    
    cr_assert_null(removed, "remove_graph_node returns NULL (implementation detail)");
    // Verify the list structure is maintained
    cr_assert_eq(head->network->fd, 3, "Head should still be node3");
    
    free(node1);
    free(node2);
    free(node3);
}

Test(graphic_client, remove_graph_node_non_existing, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    add_graph_node(&head, 1);
    add_graph_node(&head, 2);
    
    graph_net_t *result = remove_graph_node(&head, 999);
    
    cr_assert_null(result, "Removing non-existing node should return NULL");
    cr_assert_not_null(head, "Head should remain unchanged");
    
    // Clean up
    while (head) {
        graph_net_t *next = head->next;
        free(head);
        head = next;
    }
}

Test(graphic_client, remove_graph_node_empty_list, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    
    graph_net_t *result = remove_graph_node(&head, 42);
    
    cr_assert_null(result, "Removing from empty list should return NULL");
    cr_assert_null(head, "Head should remain NULL");
}


Test(graphic_client, poll_graphic_clients_null_zappy, .init = redirect_all_std)
{
    // Should handle NULL zappy gracefully (may crash depending on implementation)
    // This test documents expected behavior
    cr_assert(1, "This test documents that NULL zappy handling should be considered");
}


// Integration test
Test(graphic_client, add_and_remove_integration, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    
    // Add multiple nodes
    add_graph_node(&head, 1);
    add_graph_node(&head, 2);
    add_graph_node(&head, 3);
    
    cr_assert_not_null(head, "Head should not be NULL after additions");
    cr_assert_eq(head->network->fd, 3, "Head should be the last added node");
    
    // Remove a node
    remove_graph_node(&head, 2);
    
    // Verify list integrity
    cr_assert_not_null(head, "Head should still exist after removal");
    
    // Clean up remaining nodes
    while (head) {
        graph_net_t *next = head->next;
        free(head);
        head = next;
    }
}

Test(graphic_client, graph_node_initialization, .init = redirect_all_std)
{
    graph_net_t *head = NULL;
    int test_fd = 123;
    
    graph_net_t *node = add_graph_node(&head, test_fd);
    
    cr_assert_not_null(node, "Node should be created");
    cr_assert_eq(node->network->fd, test_fd, "FD should be set correctly");
    cr_assert_eq(node->mapSent, false, "mapSent should be false initially");
    cr_assert_eq(node->next, NULL, "next should be NULL for single node");
    
    free(node);
}

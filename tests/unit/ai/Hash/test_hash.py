# filepath: /home/roussierenoa/Projet/TEK_REPO/Tek2/zappy/tests/unit/ai/Hash/test_hash.py
#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_hash.py
#

import unittest
import sys
import os
from ai.src.Hash.Hash import Hash

class TestHash(unittest.TestCase):
    def setUp(self):
        self.hash_obj = Hash("test_key")
    
    def test_hash_initialization(self):
        hash_obj = Hash("test")
        self.assertIsInstance(hash_obj.key, int)
        self.assertTrue(0 <= hash_obj.key < 256)
    
    def test_simple_xor(self):
        data = b"hello"
        result = self.hash_obj.simple_xor(data)
        self.assertIsInstance(result, bytes)
        self.assertEqual(len(result), len(data))
    
    def test_hash_message(self):
        message = "hello world"
        hashed = self.hash_obj.hashMessage(message)
        self.assertIsInstance(hashed, str)
        self.assertTrue(all(c in '0123456789abcdef' for c in hashed))
        self.assertEqual(len(hashed), len(message) * 2)
    
    def test_unhash_message(self):
        message = "hello world"
        hashed = self.hash_obj.hashMessage(message)
        unhashed = self.hash_obj.unHashMessage(hashed)
        self.assertEqual(message, unhashed)
    
    def test_hash_unhash_roundtrip(self):
        test_messages = ["", "a", "test message", "123456", "special chars: !@#$%"]
        for message in test_messages:
            with self.subTest(message=message):
                hashed = self.hash_obj.hashMessage(message)
                unhashed = self.hash_obj.unHashMessage(hashed)
                self.assertEqual(message, unhashed)
    
    def test_different_keys_produce_different_hashes(self):
        hash1 = Hash("key1")
        hash2 = Hash("key2")
        message = "test message"
        hashed1 = hash1.hashMessage(message)
        hashed2 = hash2.hashMessage(message)
        self.assertNotEqual(hashed1, hashed2)

if __name__ == '__main__':
    unittest.main()

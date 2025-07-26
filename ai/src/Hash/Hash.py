#
# EPITECH PROJECT, 2025
# zappy
# File description:
# hash.py
#

class Hash:
    def __init__(self, hash_key: str):
        self.key = sum((i + 1) * ord(c) for i, c in enumerate(hash_key)) % 256

    def simple_xor(self, data: bytes) -> bytes:
        return bytes((b ^ (self.key % 256)) for b in data)

    def hashMessage(self, message: str) -> str:
        data = message.encode()
        hashed = self.simple_xor(data)
        return "".join(f"{b:02x}" for b in hashed)

    def unHashMessage(self, hex_message: str) -> str:
        try:
            if len(hex_message) % 2 != 0:
                raise ValueError("Hex string must have even length")

            encrypted = bytes(
                int(hex_message[i:i + 2], 16) for i in range(0, len(hex_message), 2)
            )
            decrypted = self.simple_xor(encrypted)
            return decrypted.decode()
        except (ValueError, UnicodeDecodeError) as e:
            raise ValueError(f"Failed to unhash message: {e}")

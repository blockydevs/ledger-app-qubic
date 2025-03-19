import struct


class TransactionQubicError(Exception):
    pass


class TransactionQubic:
    source_public_key: bytes  # Equivalent to uint8_t[ADDRESS_LEN]
    destination_public_key: bytes  # Equivalent to uint8_t[ADDRESS_LEN]
    amount: int  # Equivalent to uint64_t
    tick: int  # Equivalent to uint32_t
    input_type: int  # Equivalent to uint16_t
    input_size: int  # Equivalent to uint16_t

    ADDRESS_LEN = 32  # Example length for the public keys

    def __init__(self, source_public_key: bytes, destination_public_key: bytes,
                 amount: int, tick: int, input_type: int, input_size: int):
        self.source_public_key = source_public_key
        self.destination_public_key = destination_public_key
        self.amount = amount
        self.tick = tick
        self.input_type = input_type
        self.input_size = input_size

    def serialize(self) -> bytes:
        """
        Serialize the object into bytes.
        """
        # Format string construction
        format_string = f'{self.ADDRESS_LEN}s'  # source_public_key
        format_string += f'{self.ADDRESS_LEN}s'  # destination_public_key
        format_string += 'Q'  # uint64_t (amount)
        format_string += 'I'  # uint32_t (tick)
        format_string += 'H'  # uint16_t (input_type)
        format_string += 'H'  # uint16_t (input_size)

        # Packing the data
        packed_data = struct.pack(
            format_string,
            self.source_public_key,
            self.destination_public_key,
            self.amount,
            self.tick,
            self.input_type,
            self.input_size
        )

        return packed_data

    @classmethod
    def from_bytes(cls, data: bytes):
        """
        Deserialize bytes into a TransactionQubic object.
        """
        # Format string construction for unpacking
        format_string = f'{cls.ADDRESS_LEN}s'  # source_public_key
        format_string += f'{cls.ADDRESS_LEN}s'  # destination_public_key
        format_string += 'Q'  # uint64_t (amount)
        format_string += 'I'  # uint32_t (tick)
        format_string += 'H'  # uint16_t (input_type)
        format_string += 'H'  # uint16_t (input_size)

        # Unpacking the data
        unpacked = struct.unpack(format_string, data)

        # Returning the deserialized object
        return cls(
            source_public_key=unpacked[0],
            destination_public_key=unpacked[1],
            amount=unpacked[2],
            tick=unpacked[3],
            input_type=unpacked[4],
            input_size=unpacked[5],
        )

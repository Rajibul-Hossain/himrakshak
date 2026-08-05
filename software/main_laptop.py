# main_laptop.py
import time
from abc import ABC, abstractmethod
from packet import TelemetryData, build_ota_frame

# --- Hardware Abstraction Layer ---

class LoRaTransmitter(ABC):
    @abstractmethod
    def transmit(self, data: bytes):
        pass

class MockLoRa(LoRaTransmitter):
    """Used for testing on a laptop without physical GPIO/SPI."""
    def transmit(self, data: bytes):
        print(f"\n[MOCK LORA] Transmitting {len(data)} bytes:")
        # Print as professional hex dump
        hex_dump = ' '.join(f'{b:02X}' for b in data)
        print(f"RAW OTA FRAME: {hex_dump}")
        
        # Analyze frame structure for validation
        print(f"├─ Header (Plain) : {hex_dump[:5]}")
        print(f"├─ IV     (Plain) : {hex_dump[6:53]}")
        print(f"└─ Payload (AES)  : {hex_dump[54:]}")
def main():
    lora = MockLoRa()
    
    packet_counter = 1
    
    print("Starting Secure Telemetry System (Laptop Mock Mode)...")
    
    try:
        while True:
            current_time = int(time.time())
            sensor_data = TelemetryData(
                counter=packet_counter,
                timestamp=current_time,
                temperature=24.5,       # Mock data
                humidity=60.2,          # Mock data
                battery_mv=4100,        # 4.1V Mock battery
                flags=0x00              # System OK
            )
            
            # 2. Build and Encrypt the Packet
            ota_packet = build_ota_frame(sensor_data)
            
            # 3. Transmit
            lora.transmit(ota_packet)
            
            packet_counter += 1
            time.sleep(3) # Wait 3 seconds before next transmission
            
    except KeyboardInterrupt:
        print("\nSystem halted by user.")

if __name__ == "__main__":
    main()
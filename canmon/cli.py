import argparse
import logging
import os

def main():
	parser = argparse.ArgumentParser(description='Interact with a CANmon USB CAN bus monitoring board')

	parser.add_argument('device', help='device to connect to (e.g. /dev/ttyusb1)')

	args = parser.parse_args()

	print("Hello, World!")
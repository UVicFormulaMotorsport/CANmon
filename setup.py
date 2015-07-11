from setuptools import setup

version = __import__('canmon').VERSION

setup(
	name = 'canmon',
	version = version,
	url = 'https://github.com/darrylring/CANmon',
	author = 'Darryl Ring',
	author_email = 'darryl@darrylring.com',
	license = 'MIT',

	packages = ['canmon']
)
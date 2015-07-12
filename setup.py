from setuptools import setup

version = __import__('canmon').VERSION

setup(
	name = 'CANmon',
	version = version,
	url = 'https://github.com/darrylring/canmon',
	author = 'Darryl Ring',
	author_email = 'darryl@darrylring.com',
	license = 'MIT',

	packages = ['canmon'],
	entry_points = {
		'console_scripts': [
			'canmon = canmon.cli.__init__:main',
		]
	},

	install_requires = [
		'pyserial'
	]
)
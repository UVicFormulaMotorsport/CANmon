from setuptools import setup

version = __import__('canmon').VERSION

setup(
	name = 'CANmon',
	version = version,
	url = 'https://github.com/UVicFormulaMotorsport/CANmon',
	author = 'Darryl Ring',
	author_email = 'darryl@darrylring.com',
	license = 'MIT',

	packages = ['canmon'],
	entry_points = {
		'console_scripts': [
			'canmon = canmon.cli:main',
		]
	},

	install_requires = [
		'flask',
		'pyserial',
		'requests'
	]
)

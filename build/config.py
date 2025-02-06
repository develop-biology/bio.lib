import eons
import shutil

class config(eons.StandardFunctor):
	def __init__(this, name="Build Config for bio.lib"):
		super().__init__(name)
		this.functionSucceeded = True
		this.rollbackEnabled = False

		this.optionalKWArgs['cpp_versions'] = [
			'98',
			'11',
			'17',
			'20'
		]

		this.optionalKWArgs['toolchains'] = [
			'arm'
		]
	
	def Function(this):
		shutil.rmtree('tmp', ignore_errors=True)
		this.Copy('../src', 'tmp/src')
		this.Copy('../inc', 'tmp/inc')

		this.result.data.update({
			"build": "cpp",
			"name": "bio",
			"type": "lib",
			"path": "tmp",
			"build_in": "bio",
			"file_name": "bio.lib",
			"cpp_versions": this.cpp_versions,
			"toolchains": this.toolchains,
		})
	
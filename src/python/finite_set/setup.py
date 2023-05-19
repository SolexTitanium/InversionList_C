import os
import shutil
from setuptools import Extension, Distribution
from Cython.Build import cythonize
from Cython.Distutils import build_ext


def build():
    extensions = [
        Extension("inversion_list", ["inversion_list.pyx"]),
    ]

    ext_modules = cythonize(
        extensions,
        compiler_directives={"binding": True, "language_level": 3},
    )

    distribution = Distribution({"name": "finite_set", "ext_modules": ext_modules})
    distribution.package_dir = "finite_set"

    cmd = build_ext(distribution)
    cmd.ensure_finalized()
    cmd.run()

    # Copy built extensions back to the project
    for output in cmd.get_outputs():
        relative_extension = os.path.relpath(output, cmd.build_lib)
        shutil.copyfile(output, relative_extension)
        mode = os.stat(relative_extension).st_mode
        mode |= (mode & 0o444) >> 2
        os.chmod(relative_extension, mode)

if __name__ == "__main__":
    build()


from setuptools import setup, Extension

setup(
    name='mac_display',
    author='rob@sixty-north.com',
    author_email="rob@sixty-north.com",
    description="Retrieve information about macOS displays",
    version='1.0',
    ext_modules=[
        Extension(
            'mac_display',
            ['module.c', 'display.c'],
            extra_link_args = [
                '-framework', 'IOKit',
                '-framework', 'CoreFoundation',
                '-framework', 'ApplicationServices'
            ]
        )
    ],
)

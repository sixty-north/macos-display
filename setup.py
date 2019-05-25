from setuptools import setup, Extension

setup(
    name='mac_display',
    author='rob@sixty-north.com',
    author_email="rob@sixty-north.com",
    description="Retrieve information about macOS displays",
    version='1.0',
    packages = ['mac_display'],
    ext_modules=[
        Extension(
            'mac_display.display',
            ['mac_display/display_module.c',
             'mac_display/display.c'],
            extra_link_args = [
                '-framework', 'IOKit',
                '-framework', 'CoreFoundation',
                '-framework', 'ApplicationServices'
            ]
        )
    ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Operating System :: MacOS :: MacOS X',
        'Programming Language :: Python :: 3',
    ]
)

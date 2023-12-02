#python2 setup.py sdist bdist_wheel

# For testing
#twine upload --repostitory testpypi dist/*
#pip install --index-url https://test.pypi.org/simple/ --no-deps multiio

# For release
#twine upload dist/*
#pip install multiio


#with open("README.md", 'r') as f:
#    long_description = f.read()

from setuptools import setup, find_packages
setup(
    name='SMmegaind',
    packages=find_packages(),
    version='1.0.0',
    license='MIT',
    description='Library to control PLACEHOLDER Automation Card',
    #long_description=long_description,
    #long_description_content_type="text/markdown",
    author='Sequent Microsystems',
    author_email='olcitu@gmail.com',
    url='https://sequentmicrosystems.com',
    #keywords=['industrial', 'raspberry', 'power', '4-20mA', '0-10V', 'optoisolated'],
    install_requires=[
        "smbus2",
        ],
    classifiers=[
        'Development Status :: 4 - Beta',
        # Chose either "3 - Alpha", "4 - Beta" or "5 - Production/Stable" as the current state of your package
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        ],
    )

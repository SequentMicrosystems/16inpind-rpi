# Contributing to Sequent Microsystems Python Libraries

Thank you for your interest in contributing to Sequent Microsystems' Python libraries! Since these libraries share a similar structure, improvements to one can often benefit others.

These libraries fall into two categories:
1. Function-based libraries (older structure)
2. Class-based libraries (with initialization)

Please review the structure of the current library or similar libraries before making changes to ensure consistency.

## Documentation

Most libraries use Google-style Python docstrings to generate documentation via the Python Makefile.
Ensure that Sphinx and the Markdown extension are installed:

```bash
pip install -U sphinx sphinx-markdown-builder
```

Once installed, generate the documentation from the `docs/conf.py` and `docs/index.rst` by running:
```bash
make
```

## Packaging and Uploading

Once your changes are complete and everything is working, submit a pull request to the GitHub repository.

# PyPI Upload (Admin Only)

Only PyPI admins can proceed with publishing. The Makefile includes the packaging process. When the setup.py file is ready, run:

```bash
make
```

If everything completes successfully, upload the package to PyPI with:

```bash
twine upload dist/*
```
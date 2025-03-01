Welcome to the SM16inpind Documentation!
========================================

SM16inpind is a Python package that enables you to control the
`Sixteen LV Digital Inputs <https://sequentmicrosystems.com/products/sixteen-lv-digital-inputs-card-for-raspberry-pi>`_
for Raspberry Pi.

This documentation provides detailed instructions on installation, usage, and troubleshooting.

If you experience any issues, please refer to the :ref:`troubleshooting` section.

**Note:** On older systems, the `pip` command may default to Python 2.7, which is no longer supported. In these cases, use `pip3` instead.

If you are working in an externally managed environment, we recommend creating a virtual environment to manage dependencies:

.. code-block:: bash

    python3 -m venv .venv
    source .venv/bin/activate


Installation
============

Install the SM16inpind package using one of the following commands:

.. code-block:: bash

    sudo pip install SM16inpind

or

.. code-block:: bash

    sudo pip3 install SM16inpind


Updating an Existing Installation
=================================

To update SM16inpind to the latest version, run:

.. code-block:: bash

    sudo pip install SM16inpind -U

or

.. code-block:: bash

    sudo pip3 install SM16inpind -U


Quick Start Example
===================

Start using SM16inpind by launching a Python interpreter:

.. code-block:: console

   $ python
   Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on Linux
   Type "help", "copyright", "credits" or "license" for more information.
   >>> import lib16inpind
   >>> lib16inpind.getLed(0, 1)
   >>>

.. Start using SM16inpind by launching a Python interpreter and initiate the class:
.. 
.. .. code-block:: console
.. 
..    $ python
..    Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on Linux
..    Type "help", "copyright", "credits" or "license" for more information.
..    >>> import multiio
..    >>> mu = multiio.SMmultiio()
..    >>> mu.set_u_out(0, 1)
..    >>>


Full Documentation
==================

For a comprehensive guide to the package's functionality, please refer to the contents below:

.. toctree::
    :maxdepth: 2
    :caption: Contents:

.. automodule:: lib16inpind
    :members:
    :undoc-members:
    :show-inheritance:
    :member-order: bysource


.. _troubleshooting:

Troubleshooting
===============

If you encounter any problems, first verify that your package is up to date. To check the installed version, run:

.. code-block:: bash

    pip freeze | grep -i SM16inpind

If you installed SM16inpind with pip3, use:

.. code-block:: bash

    pip3 freeze | grep -i SM16inpind

Additionally, if you are using a virtual environment, ensure it is activated by checking the Python interpreter's path:

.. code-block:: bash

    which python

If the output does not show the path to your virtual environment, activate it with:

.. code-block:: bash

    source /path/to/.venv/bin/activate

.. vi:se ts=4 sw=4 et:
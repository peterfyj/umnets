Umnets
======

Implemented in c++11, Umnets provides a framework for theoretical MANETs simulations. You can implement your own MANET models based on this framework.

Requirements
------------

GCC >= 4.6.2
Boost >= 1.32.0

Basics
------

### Make

Change the settings in `Setting.mk` to choose the combination of your simulation models. After that, simply run `make` in the root directory and the binary `umnets` is expected to show up.

### Run

You can run `./umnets -c config.cfg` to specify the configuration file being used. The default configuration file would be config.cfg. The configuration is specified by each of the model you choose in `Setting.mk`.

### Further help

You can run `./umnets -h` for more help.


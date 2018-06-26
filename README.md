# se84_dp

**WORK IN PROGRESS and most probably still quite buggy**

A module for [luaXroot](https://github.com/zupalex/luaXroot) designed to treat data acquired during the 84Se(d,p) experiment performed at NSCL in December 2017.

This module cannot work on its own and requires the module [nscl_unpacker](https://github.com/zupalex/nscl_unpacker). Download both modules into the *user* folder of luaXroot. It also requires the experimental Python GUI feature of luaXroot to be enabled.

Add to *user/userpyscripts.py* the following lines

Before `def load_user_py_scripts(master):`
```
from se84_dp.root_converter_gui import *
```

Inside `def load_user_py_scripts(master):`
```
add_evt_converter_button(master)
```

Without any other user python scripts loaded, your *user/userpyscripts.py* should look like this

```
from se84_dp.root_converter_gui import *

def load_user_py_scripts(master):
    add_evt_converter_button(master)
```

The final data can either be sorted in a [ROOT](https://root.cern.ch/) [TTree](https://root.cern.ch/doc/master/classTTree.html) or displayed in histograms.

No detailed documentation is provided as this module is designed specifically for that experiment. However it can be used as an example of what can be done.

A basic documentation will be made available eventually.

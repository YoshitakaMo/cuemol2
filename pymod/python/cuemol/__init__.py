import os
from pathlib import Path

from cuemol.internal_loader import import_internal

# print("cuemol/__init__.py called", here)

ci = import_internal()
if not ci.isInitialized():
    conf_path = os.getenv("CUEMOL_SYSCONFIG_PATH")
    if conf_path is None:
        ci.initCueMol()
    else:
        conf_path = Path(conf_path)
        ci.initCueMol(str(conf_path / "sysconfig.xml"))

from cuemol.cuemol import *  # NOQA

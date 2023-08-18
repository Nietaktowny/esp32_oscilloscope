Import("env")
import subprocess
import os

# Path to where desktop tests jsons are placed.
if not os.path.exists(".pio/tests"):
    os.mkdir(".pio/tests")
if not os.path.exists(".pio/tests/desktop"):
    os.mkdir(".pio/tests/desktop")
if not os.path.exists(".reports"):
    os.mkdir(".reports")

def gcovr_callback(*arg, **kwargs):
    print("Executing gcovr")
    env.Execute("gcovr --add-tracefile .pio/tests/desktop/*.json --html-details .reports/coverage.html")

env.AddCustomTarget(
    "gcovr",
    None,
    gcovr_callback,
    title="gcovr",
    description="Executes gcovr")


def runNativeChecks(source, target, env):
    print("Running gcovr", source, target)
    test_name = env.get("PIOTEST_RUNNING_NAME", "");
    print("Name", env.get("PIOTEST_RUNNING_NAME", ""), target[0].rstr())
    os.makedirs(".pio/tests/", exist_ok=True)
    cppcheckargs = ["gcovr", "...","--json", ".pio/tests/" + test_name + ".json"]
    env.Execute(" ".join(cppcheckargs))

env.AddPostAction("$BUILD_DIR/$PROGNAME$PROGSUFFIX", runNativeChecks)
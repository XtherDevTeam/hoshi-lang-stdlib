import os
import sys
import subprocess
import pathlib

TEST_CASES = {}


def run_test(test_file):
    test_file = pathlib.Path(test_file)
    test_name = test_file.stem
    test_info = TEST_CASES.get(test_name, {"type": "executable"})

    build_type = test_info["type"]
    output_path = "build-release/" + test_name
    params = ["make", "./build-release/" + test_name]
    
    res = subprocess.run(params)
    if res.returncode != 0:
        print(f"ERR: Compiler test failed for case: {test_name}")
        return False
    else:
        print(f"OK: Compiler test passed for case: {test_name}")

    if build_type == "executable":
        res = subprocess.run([output_path])
        expected_return_code = test_info.get("return_code")
        if expected_return_code is not None and res.returncode != expected_return_code and sys.platform != "win32": # disable code run on windows, fuck i16 return code
            print(f"ERR: Execution test failed for case: {test_name}. Expected {expected_return_code}, got {res.returncode}")
            return False
        else:
            print(f"OK: Execution test passed for case: {test_name}")
    
    return True


def get_test_files(examples: pathlib.Path = pathlib.Path("examples")):
    test_files = []
    for file in examples.iterdir():
        if file.is_file() and file.suffix == ".hoshi":
            test_files.append(file)
    return test_files


def compile_project():
    if not os.path.exists("cmake-build-release"):
        os.makedirs("cmake-build-release")
    if subprocess.run(["make", "cmake_production"]).returncode != 0:
        print("ERR: CMake configuration failed")
        return False
    if subprocess.run(["make", "build_production"]).returncode != 0:
        print("ERR: Make build failed")
        return False
    return True


if __name__ == "__main__":
    if not os.path.exists("build-release"):
        os.makedirs("build-release")
    if not os.path.exists("cmake-build-release"):
        os.makedirs("cmake-build-release")
    
    test_files = get_test_files()
    ok = 0
    failing = []
    
    print("Compiling project...")
    if not compile_project():
        exit(1)
    
    print("Running automatic code test...")
    for test_file in test_files:
        if run_test(test_file):
            ok += 1
        else:
            failing.append(test_file.stem)

    print(f"Automatic code test finished. Passed: {ok}/{len(test_files)}")
    if failing:
        print("Failed cases: " + ", ".join(failing))
        exit(1)
        
    exit(0)
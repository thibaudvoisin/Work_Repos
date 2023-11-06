from argparse import ArgumentParser # import for parsing
from pathlib import Path # import for absolute path in run_test ...
from difflib import unified_diff # pretty print diff
from dataclasses import dataclass # to make class with yml file

# colors and font import ^^
from alive_progress import alive_bar
import time
from termcolor import cprint 
from pyfiglet import figlet_format

import subprocess as sp
import yaml

class color:
    RST = '\033[0m' # reset
    OK_GREEN = '\033[1;92m' # light green bold
    KO_RED = '\033[1;91m' # light red bold
    BLUE = '\033[1;96m' # light blue bold
    YELLOW = '\033[3;93m' # light yellow italic
    ORANGE = '\033[38;5;202m' # custom orange
    MAGENTA = '\033[1;95m' # magenta

OK_TAG = f"[ {color.OK_GREEN}SUCCESS{color.RST} ]"
KO_TAG = f"[ {color.KO_RED}FAILURE{color.RST} ]"

# yml dataclass
@dataclass
class TestCase:
    week: str
    name: str
    input: str


# pretty diff print
def diff(expected: str, actual: str) -> str:
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)

    return ''.join(unified_diff(expected_lines, actual_lines,
        fromfile='expected', tofile='actual'))


# run 42sh and dash
def run_shell(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True, timeout=5)


# test all output
def perform_check(expected: sp.CompletedProcess, actual: sp.CompletedProcess):
     assert expected.returncode == actual.returncode, \
            f"{diff(str(expected.returncode), str(actual.returncode))}"

     assert expected.stdout == actual.stdout, \
            f"{diff(expected.stdout, actual.stdout)}"


def main():
    # check if the binqry is good and parse it
    parser = ArgumentParser('Testsuite')
    parser.add_argument('--binary', required=True, type=Path)

    args = parser.parse_args()
    binary_path = args.binary.absolute()

    # get data from yml file
    with open("tests/data.yml", 'r') as file:
        testsuite = [TestCase(**testcase) for testcase in yaml.safe_load(file)]

    testnumber, testpassed = len(testsuite), 0

    print('\n\n')
    cprint(figlet_format('42sh', font='colossal'), 'red', attrs=['bold'], end='')

    week = 1


    with alive_bar(testnumber, force_tty=True, enrich_print=False, length=60) as bar:
        for testcase in testsuite:
            curr_week = testcase.week
            stdin = testcase.input
            name = testcase.name

            # run dash
            dash_proc = run_shell("dash", stdin)
            # run 42sh
            sh_proc = run_shell(binary_path, stdin)

            time.sleep(.05)

            if week == curr_week:
                if week != 1:
                    print("\n")
                print(f'{color.MAGENTA}#{color.RST}[{color.BLUE} WEEK {week} {color.RST}]')
                print("\n")
                week += 1

            if name[0] == '(':
                name = f"{color.ORANGE}{name}{color.RST}"

            try:
                perform_check(dash_proc, sh_proc)
            except AssertionError as err:
                print(f"{color.MAGENTA}#{color.RST}{KO_TAG} {color.YELLOW}{name}{color.RST}\n{err}")
            else:
                testpassed += 1
                print(f"{color.MAGENTA}#{color.RST}{OK_TAG} {color.YELLOW}{name}{color.RST}")
                bar()
        print("\n")

    testfailure = testnumber - testpassed
    ratio = (testpassed / testnumber) * 100

    if ratio >= 50:
        print(f"\nRatio of tests: {color.OK_GREEN}{format(ratio, '.2f')}%{color.RST} !")
    else:
        print(f"\nRatio of tests: {color.KO_RED}{ratio}%{color.RST}...")

    print(f"\n{color.OK_GREEN}{testpassed}{color.RST} test(s) passed on {color.BLUE}{testnumber}{color.RST} test(s) (ie: {color.KO_RED}{testfailure}{color.RST} failure(s)).")


if __name__ == '__main__':
    main()

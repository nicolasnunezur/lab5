#!/usr/bin/env python3
import argparse
from pathlib import Path
import subprocess
import sys
import termcolor

def main():
    ap = argparse.ArgumentParser(description="Feed commands to a console game and report if it finished.")
    ap.add_argument("game", help="Path to the game executable (e.g., ./game)")
    ap.add_argument("--timeout", type=float, default=0.25,
                    help="Seconds to wait for the game to exit after sending input (default: 0.25)")
    args = ap.parse_args()

    inputs = Path(args.game).with_suffix(".solution")

    # Read commands; ignore empty lines and lines starting with '#'
    with open(inputs, encoding="utf-8") as f:
        cmds = [ln.strip() for ln in f if ln.strip() and not ln.lstrip().startswith("#")]

    # All commands as a single stdin payload (final newline included)
    payload = ("\n".join(cmds) + "\n") if cmds else "\n"

    try:
        # Send input and wait for exit (headless: no output shown)
        res = subprocess.run(
            [args.game],
            input=payload,
            text=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            timeout=args.timeout,
        )
    except subprocess.TimeoutExpired:
        # Didn't finish after feeding input within the timeout
        print(termcolor.colored("GAME NOT FINISHED", "yellow"))
        sys.exit(1)
    except FileNotFoundError:
        print(termcolor.colored(f"ERROR: game not found: {args.game}", "red"))
        sys.exit(2)

    # If we got here, the process exited on its own
    print(termcolor.colored("SUCCESS!", "green"))
    sys.exit(0 if res.returncode == 0 else res.returncode)

if __name__ == "__main__":
    main()

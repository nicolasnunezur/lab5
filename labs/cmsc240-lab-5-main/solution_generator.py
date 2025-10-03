# Vibe coded with ChatGPT

#!/usr/bin/env python3
import argparse
import os
import signal
import subprocess
import sys
import time
from threading import Thread, Lock
import random
import shutil
from pathlib import Path

def stream_output(proc, log_fp=None, log_lock=None):
    """Headless: capture child's stdout ONLY to the log (no console printing)."""
    try:
        for line in proc.stdout:
            if log_fp and log_lock:
                with log_lock:
                    log_fp.write(line)
                    log_fp.flush()
    except Exception:
        pass  # Pipe may close on termination; that's fine.

def main():
    ap = argparse.ArgumentParser(
        description="Headless runner: feed random commands to a C++ console game. Stop after first successful finish (log saved only then)."
    )
    ap.add_argument("game", help="Path of the game executable to run (e.g., ./game).")
    ap.add_argument("max_inputs", default=200, type=int, help="Maximum number of inputs to send per attempt.")
    ap.add_argument("attempts", default=10, type=int, help="Maximum number of attempts.")
    ap.add_argument("--delay", type=float, default=0.01,
                    help="Seconds to wait after writing each command (default: 0.01).")
    ap.add_argument("--finish-grace", type=float, default=0.4,
                    help="Seconds to allow the game to exit on its own after feeding inputs (default: 0.4).")
    ap.add_argument("--cwd", default=None,
                    help="Optional working directory to launch the game in.")
    args = ap.parse_args()

    if not os.path.exists(args.game):
        ap.error(f"Game not found: {args.game}")

    input_file = Path(args.game).with_suffix(".inputs")
    if not os.path.exists(input_file):
        ap.error(f"Inputs file not found: {input_file}")

    # Read inputs (ignore blanks and comments)
    with open(input_file, "r", encoding="utf-8") as f:
        all_inputs = [ln.strip() for ln in f]
    all_inputs = [c for c in all_inputs if c and not c.lstrip().startswith("#")]
    if not all_inputs:
        ap.error("The inputs file has no usable commands.")

    for attempt in range(1, args.attempts + 1):
        print(f"[attempt {attempt}] starting...")
        success, saved_path = run_game_instance(args, all_inputs, attempt, args.attempts)
        if success:
            print(f"[success] attempt {attempt} — saved log to: {saved_path}")
            sys.exit(0)

    print("[no-success] all attempts ended without a natural finish; no logs saved.")
    sys.exit(1)

def numbered_log_path(base_path, attempt, attempts_total):
    """
    Returns (final_log_path, tmp_log_path).
    With multiple attempts, append '-attemptNN' before the extension.
    Always write to a '.tmp' file first and promote only on success.
    """
    p = Path(base_path)
    # if attempts_total > 1:
    #     stem = p.stem
    #     suf = "".join(p.suffixes) if p.suffixes else ""
    #     final = p.with_name(f"{stem}-attempt{attempt:02d}{suf}")
    # else:
    final = p
    tmp = final.with_suffix(final.suffix + ".tmp") if final.suffix else Path(str(final) + ".tmp")
    return str(final), str(tmp)

def run_game_instance(args, all_inputs, attempt_idx, attempts_total):
    # Pick a randomized list of commands for this attempt
    commands = [random.choice(all_inputs) for _ in range(args.max_inputs)]

    # Per-attempt log paths
    log_file = Path(args.game).with_suffix(".log")
    log_file = "AI-" + str(log_file)
    final_log_path, tmp_log_path = numbered_log_path(log_file, attempt_idx, attempts_total)

    # Open temp log (only promoted on success)
    Path(final_log_path).parent.mkdir(parents=True, exist_ok=True)
    log_fp = open(tmp_log_path, "w", encoding="utf-8")
    log_lock = Lock()

    def log_input_line(s):
        with log_lock:
            log_fp.write(f"%% {s}\n")
            log_fp.flush()

    # Launch the game headlessly in its own process group
    proc = subprocess.Popen(
        [args.game],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,               # line-buffered
        cwd=args.cwd,
        preexec_fn=os.setsid     # POSIX: new session so we can signal the whole group
    )

    # Stream output ONLY to log
    t = Thread(target=stream_output, args=(proc, log_fp, log_lock), daemon=True)
    t.start()

    # wait a bit for the game to initialize (optional)
    time.sleep(1)

    log_input_line(f"%% [GAME START]")

    signaled = False

    try:
        for cmd in commands:
            if proc.poll() is not None:
                break  # game already exited
            proc.stdin.write(cmd + "\n")
            proc.stdin.flush()
            log_input_line(cmd)
            time.sleep(args.delay)

        # Let the game finish naturally if it will
        waited = 0.0
        step = 0.05
        while proc.poll() is None and waited < args.finish_grace:
            time.sleep(step)
            waited += step

        # If still running, stop it (won't count as "finished")
        if proc.poll() is None:
            try:
                os.killpg(proc.pid, signal.SIGINT)
                signaled = True
            except ProcessLookupError:
                pass

            try:
                proc.wait(timeout=1)
            except subprocess.TimeoutExpired:
                try:
                    os.killpg(proc.pid, signal.SIGTERM)
                    signaled = True
                except ProcessLookupError:
                    pass
                try:
                    proc.wait(timeout=1)
                except subprocess.TimeoutExpired:
                    try:
                        os.killpg(proc.pid, signal.SIGKILL)
                        signaled = True
                    except ProcessLookupError:
                        pass
                    try:
                        proc.wait(timeout=1)
                    except subprocess.TimeoutExpired:
                        pass

    finally:
        # Close stdin and wait
        try:
            if proc.stdin:
                proc.stdin.close()
        except Exception:
            pass
        try:
            proc.wait(timeout=2)
        except subprocess.TimeoutExpired:
            pass

        t.join(timeout=1)

        finished_naturally = (proc.returncode is not None) and (not signaled)

        # Write final marker then close
        with log_lock:
            if finished_naturally:
                log_fp.write(f"\n%% [GAME FINISHED rc={proc.returncode}]\n")
            else:
                log_fp.write(f"\n%% [GAME NOT FINISHED]\n")
            log_fp.flush()
        log_fp.close()

        # Promote or discard the temp log
        if finished_naturally:
            shutil.move(tmp_log_path, final_log_path)
            return True, final_log_path
        else:
            try:
                os.remove(tmp_log_path)
            except FileNotFoundError:
                pass
            return False, None

if __name__ == "__main__":
    main()

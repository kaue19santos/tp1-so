#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
IMAGE_NAME=tp1-so-xv6:latest

podman --cgroup-manager=cgroupfs build -f "$SCRIPT_DIR/Containerfile" -t "$IMAGE_NAME" "$REPO_ROOT"

podman --cgroup-manager=cgroupfs run --rm -it \
  --userns=keep-id \
  -v "$REPO_ROOT:/work" \
  -w /work \
  "$IMAGE_NAME" \
  bash -lc 'make clean && make kernel/kernel fs.img && make qemu'

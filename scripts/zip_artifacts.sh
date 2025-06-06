#!/usr/bin/env bash
set -e

PLUGIN_NAME="SimpleGlitch2"
ARTIFACTS_DIR="artifacts"
OUTPUT_DIR="release"

mkdir -p "${OUTPUT_DIR}"

for ver_path in "${ARTIFACTS_DIR}"/*; do
  version=$(basename "$ver_path")
  linux_so="$ver_path/Linux/${PLUGIN_NAME}.so"
  win_dll="$ver_path/Windows/${PLUGIN_NAME}.dll"

  # zip linux
  if [[ -f "$linux_so" ]]; then
    zip_name="${PLUGIN_NAME}-Linux-${version}.zip"
    echo "→ Empaquetando Linux ${version} en ${zip_name}..."
    zip -j "${OUTPUT_DIR}/${zip_name}" "$linux_so"
    echo "   ✓ Creado: ${OUTPUT_DIR}/${zip_name}"
  else
    echo "⚠ Saltando Linux ${version}: no se encontró ${linux_so}"
  fi

  # zip windows
  if [[ -f "$win_dll" ]]; then
    zip_name="${PLUGIN_NAME}-Windows-${version}.zip"
    echo "→ Empaquetando Windows ${version} en ${zip_name}..."
    zip -j "${OUTPUT_DIR}/${zip_name}" "$win_dll"
    echo "   ✓ Creado: ${OUTPUT_DIR}/${zip_name}"
  else
    echo "⚠ Saltando Windows ${version}: no se encontró ${win_dll}"
  fi
done

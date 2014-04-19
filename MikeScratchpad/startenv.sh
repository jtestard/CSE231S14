BASESCRIPT=$(readlink -f "$0")
BASE=$(dirname "$BASESCRIPT")
PROJ="cse231"

if [ -n "${VTENV}" ]; then
    if [ "${BASE}" = "${VTENV}" -a "${PROJ}" = "${VTENV_NAME}" ]; then
        echo "${VTENV_NAME} Virtual Environment is already active."
    else
        echo "There is already ${VTENV_NAME} Virtual Environment activated."
        echo "(at ${VTENV})"
        echo "Deactivate it first (using command 'deactivate_vtenv'), to activate"
        echo "test environment."
    fi
    return 1
fi

export VTENV="${BASE}"
export VTENV_NAME="${PROJ}"
export CSE231ROOT="${VTENV}"
export LLVMBIN="${VTENV}/llvm/build/bin"
export LLVMLIB="${VTENV}/llvm/build/lib"
export BENCHMARKS="${VTENV}/extra/benchmarks"
export INSTRUMENTATION="${VTENV}/extra/instrumentation"
export OUTPUTLOGS="${VTENV}/logs"

echo "Activating ${VTENV_NAME} Virtual Environment (at ${VTENV})."
echo ""
echo "To exit from this virtual environment, enter command 'deactivate_vtenv'."

export "VTENV_PATH_BACKUP"="${PATH}"
export "VTENV_PS1_BACKUP"="${PS1}"

deactivate_vtenv() {
    echo "Deactivating ${VTENV_NAME} Virtual Environment (at ${VTENV})."
    echo "Restoring previous environment settings."

    export "PATH"="${VTENV_PATH_BACKUP}"
    unset -v "VTENV_PATH_BACKUP"
    export "PS1"="${VTENV_PS1_BACKUP}"
    unset -v "VTENV_PS1_BACKUP"

    unset -v VTENV
    unset -v VTENV_NAME
    unset -v CSE231ROOT
    unset -v LLVMBIN
    unset -v LLVMLIB
    unset -v BENCHMARKS
    unset -v INSTRUMENTATION
    unset -v OUTPUTLOGS
    unset -f deactivate_vtenv

    if [ -n "$BASH" -o -n "$ZSH_VERSION" ]; then
        hash -r
    fi
}

export PATH="${VTENV}/llvm/build/bin:${PATH}"

export PS1="[${VTENV_NAME}]${PS1}"

if [ -n "$BASH" -o -n "$ZSH_VERSION" ]; then
    hash -r
fi

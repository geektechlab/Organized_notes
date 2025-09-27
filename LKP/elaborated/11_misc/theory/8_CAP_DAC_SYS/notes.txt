CAP_DAC_OVERRIDE
==================

CAP_DAC_OVERRIDE: Allows a non-root user full file system access. Bypasses file read, write and execute permission check.

DAC stands for "discretionary access control"

$sudo setcap cap_dac_override+ep userapp

$getcap userapp

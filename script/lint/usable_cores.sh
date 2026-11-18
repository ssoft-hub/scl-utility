#!/usr/bin/env bash

# The cores the run may use. A container reads the count the machine has rather than the
# share it was given, so a cgroup quota, where one is set, is what bounds the scan.
usable_cores() {
    local online quota period
    online=$(getconf _NPROCESSORS_ONLN 2> /dev/null || echo 2)

    if [ -r /sys/fs/cgroup/cpu.max ]; then                    # cgroup v2
        read -r quota period < /sys/fs/cgroup/cpu.max || true
    elif [ -r /sys/fs/cgroup/cpu/cpu.cfs_quota_us ]; then     # cgroup v1
        quota=$(cat /sys/fs/cgroup/cpu/cpu.cfs_quota_us)
        period=$(cat /sys/fs/cgroup/cpu/cpu.cfs_period_us)
    fi

    # An unset quota reads `max` under v2 and `-1` under v1, and neither bounds anything.
    case "${quota:-max}${period:-x}" in *[!0-9]*) echo "$online"; return ;; esac
    [ "$period" -gt 0 ] || { echo "$online"; return; }

    quota=$((quota / period))
    [ "$quota" -ge 1 ] || quota=1
    if [ "$quota" -lt "$online" ]; then echo "$quota"; else echo "$online"; fi
}

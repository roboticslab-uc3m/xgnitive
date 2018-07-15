#!/usr/bin/env bash

set -e

package_name=ECF
repo_checkout=trunk
url="svn://ecf.zemris.fer.hr/ecf/$repo_checkout"

# https://stackoverflow.com/q/579196#comment24641886_579238
# In svn 1.9+ (Xenial or later), use this:
#   svn info --show-item revision $url
# http://subversion.apache.org/docs/release-notes/1.9.html#svn-info-item
rev=$(svn info "$url" | grep 'Revision' | awk '{ print $2; }')

repo_source_dir=~/"$package_name-$repo_checkout"
repo_cache_dir="$CACHE_DIR/$package_name-$repo_checkout"
rev_file=".last_revision"

echo "Cloning $package_name's $repo_checkout branch"

if [ ! -d "$repo_cache_dir" ] || \
   [ ! -f "$repo_cache_dir/$rev_file" ] || \
   [ ! "$(cat $repo_cache_dir/$rev_file)" = "$rev" ];
then
    echo "$package_name not in cache or not the latest revision of $repo_checkout"
    rm -rf "$repo_cache_dir"/*
    svn co "$url" "$repo_source_dir"
    (cd "$repo_source_dir" && autoreconf -fi && ./configure --prefix="$repo_cache_dir")
    make -C "$repo_source_dir" -j$(nproc)
    make -C "$repo_source_dir" -j$(nproc) check
    make -C "$repo_source_dir" install
    echo "$rev" > "$repo_cache_dir/$rev_file"
else
    echo "$package_name found in cache (rev $(cat $repo_cache_dir/$rev_file))"
fi

export ECF_DIR="$repo_cache_dir"
export LD_LIBRARY_PATH="$repo_cache_dir/lib:$LD_LIBRARY_PATH"

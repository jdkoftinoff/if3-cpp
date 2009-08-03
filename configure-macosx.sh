./configure \
 "--target-platform-macosx-universal=1" \
 "--macosx-deployment-target=10.5" \
 "--optimize=1" \
 "--includes=/Developer/SDKs/MacOSX10.5.sdk/usr/include/python2.5" \
"$@"

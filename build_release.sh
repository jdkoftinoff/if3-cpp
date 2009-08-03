(cd if2kkernel/unix && make realclean )
tar cvfz src.tgz  if2kkernel --exclude ".svn" --exclude "*.zip" --exclude "*~" --exclude "*.o" --exclude "*.d" --exclude "*.a"
rm -r -f tmp 2>/dev/null
mkdir tmp
mkdir tmp/if2kkernel-source
cd tmp/if2kkernel-source
tar xvfz ../../src.tgz
cd ..
zip -r ../if2kkernel-source.zip if2kkernel-source
cd ..
rm -r -f tmp
rm src.tgz

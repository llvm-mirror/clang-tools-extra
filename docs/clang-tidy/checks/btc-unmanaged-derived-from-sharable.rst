.. title:: clang-tidy - btc-unmanaged-derived-from-sharable

btc-unmanaged-derived-from-sharable
===================================

This check looks for local variables and fields in classes that have type derived from Sharable class and aren't created with Create..Ptr Method.
FIXME: Global vars are still missing. 

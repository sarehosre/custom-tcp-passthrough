# Introduction #

This article lists version changelogs and currently known bugs.

## Known Bugs ##
  * It says "Getting error -2,10061" instead of "-2,10060". This text will be corrected in future versions.
To submit a bug, you can use the [issue tracker](https://code.google.com/p/custom-tcp-passthrough/issues/list).


## Version 1.0.0 ##
Initial release


## Version 2.0.0 ##
  * Added diagnostic function
  * The programme will now automatically configure the loopback device as necessary. As a result, it now requires administrative permissions. If you do not want to run it as an administrator, you can always switch back to version 1.0.0 and run "netsh" as an administrator with the arguments `int ip add addr "Loop" 109.234.74.22/32 store=active`. To remove, simply reboot your PC or run netsh again with arguments `int ip delete address "Loop" addr=109.234.74.22"`
  * Added an application icon. More details can be found in file "/docs/info.txt".
  * Added statement that this currently only works for the EUR server. If you are a user of a different server and are interested in this programme, let me know.
  * Added logging to file "/ctp\_log.txt". _Please note that these logs are only viewable after closing the application._
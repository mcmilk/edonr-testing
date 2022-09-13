
## edonr-testing

1) AMD Ryzen 7 PRO 5850U - Little Endian
```
$ ./edonr-tests -i6
implementation               1k      4k     16k     64k    256k      1m      4m
edonr-old                  1121     920     968    2593    2569    2582    2562
edonr-new                  2706    2731    2734    2734    2733    2714    2680
edonr-old                  2538    2542    2570    2583    2584    2560    2544
edonr-new                  2734    2691    2725    2742    2719    2704    2707
edonr-old                  2537    2537    2578    2574    2570    2577    2563
edonr-new                  2714    2725    2722    2739    2746    2688    2716
edonr-old                  2512    2563    2578    2569    2562    2555    2573
edonr-new                  2726    2711    2712    2715    2760    2689    2734
edonr-old                  2516    2571    2546    2543    2566    2557    2553
edonr-new                  2711    2708    2713    2744    2746    2688    2723
edonr-old                  2524    2560    2567    2569    2577    2548    2560
edonr-new                  2694    2739    2689    2742    2727    2684    2692
```

2) POWER7 2.1 (pvr 003f 0201) - Big Endian
```
$ ./edonr-tests  -i6
implementation               1k      4k     16k     64k    256k      1m      4m
edonr-old                   953     972     964     958     959     961    1266
edonr-new                  1390    1412    1436    1438    1422    1437    1433
edonr-old                  1223    1254    1263    1265    1261    1267    1270
edonr-new                  1385    1403    1438    1439    1436    1436    1437
edonr-old                  1222    1254    1238    1266    1262    1267    1262
edonr-new                  1390    1426    1437    1435    1434    1445    1450
edonr-old                  1222    1255    1227    1265    1266    1267    1264
edonr-new                  1401    1432    1432    1444    1443    1438    1432
edonr-old                  1230    1258    1258    1266    1265    1267    1263
edonr-new                  1390    1428    1418    1442    1443    1442    1443
edonr-old                  1226    1253    1257    1266    1266    1268    1264
edonr-new                  1378    1427    1429    1442    1442    1439    1449
```

3) UltraSparc T5 (Niagara5) - Big Endian
```
$ ./edonr-tests  -i6
implementation               1k      4k     16k     64k    256k      1m      4m
edonr-old                   684     686     686     689     692     689     690
edonr-new                   499     503     503     504     505     504     504
edonr-old                   683     687     686     689     691     691     687
edonr-new                   498     503     503     504     505     504     504
edonr-old                   684     687     686     689     692     691     690
edonr-new                   498     503     503     504     505     504     504
edonr-old                   684     688     688     689     691     690     690
edonr-new                   499     503     503     504     505     504     504
edonr-old                   683     688     688     690     690     688     689
edonr-new                   499     503     502     504     505     504     504
edonr-old                   683     687     686     689     691     690     690
edonr-new                   498     503     503     504     505     504     504
```

| Version  | Threads |         Init        |       Dotprod       |         User        |         Sys         |       Elapsed       |      Speedup       |      Efficiency     |
|----------|---------|---------------------|---------------------|---------------------|---------------------|---------------------|--------------------|---------------------|
|  Serial  |    1    | 0.34131043654822335 | 0.20497408163265307 | 0.42373648648648643 | 0.13966428571428569 |  0.5622588832487311 |        1.0         |         1.0         |
| Parallel |    1    | 0.19048417647058827 | 0.08920195857988165 |     0.162421875     | 0.13448062015503875 |  0.2954638554216868 | 1.902970102540203  |  1.902970102540203  |
| Parallel |    2    | 0.24209847333333334 | 0.10674241891891892 |  0.2160357142857143 | 0.17315107913669064 |  0.1967883211678832 | 2.857176075855941  |  1.4285880379279705 |
| Parallel |    4    | 0.37360078504672894 | 0.12930487878787877 |  0.357865671641791  | 0.24741044776119409 |     0.153296875     | 3.6677778542369572 |  0.9169444635592393 |
| Parallel |    8    |  0.5247249302325582 | 0.19005529365079368 |  0.5649285714285714 | 0.36474820143884895 | 0.12227702702702703 | 4.598238090336089  |  0.5747797612920111 |
| Parallel |    16   |  0.4813613089430894 | 0.17451761151079137 |  0.3448380281690141 |  0.3354710144927536 | 0.11922222222222222 |  4.71605773461191  |  0.2947536084132444 |
| Parallel |    32   |  0.4964317999999999 | 0.17613975352112676 | 0.35214074074074075 | 0.34518248175182487 | 0.11832835820895521 | 4.751683296880044  | 0.14849010302750137 |
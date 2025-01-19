# VPSI-CA

This is the official simulation implementation of the VPSI-CA cryptography scheme (``Efficient Verifiable Cloud-Assisted PSI Cardinality for Privacy-Preserving Contact Tracing``) in C/C++ programming language, where the datum type is ``unsigned long long int`` (64-bit). The network communication is simulated by memory copying. 

### Time consumption computation

For time consumption computation in or after September 2024, better time consumption computation can be done. 

The recent period has witnessed the ``#include<chrono>`` reach a computation level of nanoseconds. Users can modify the time consumption computation codes in this repository to make more exact timing. 

The following codes may be useful for cross-platform universal improvements. 

```
#if defined WIN32 || defined _WIN32 || defined _WIN64
#include <windows.h>
#ifndef TIME_POINT_TYPE
#define TIME_POINT_TYPE chrono::steady_clock::time_point
#endif
#else
#include <string.h>
#include <math.h>
#ifndef TIME_POINT_TYPE
#define TIME_POINT_TYPE chrono::system_clock::time_point
#endif
#endif
```

```
const TIME_POINT_TYPE startTime = chrono::high_resolution_clock::now();
long long int timeDelta = (chrono::high_resolution_clock::now() - startTime).count();
cout << "Time: " << timeDelta << " ns" << endl;
```

### Citation

If you wish to cite this work, please use the following BibTeX. 

```
@article{chen2024efficient,
  title={Efficient Verifiable Cloud-Assisted PSI Cardinality for Privacy-Preserving Contact Tracing},
  author={Chen, Yafeng and Wu, Axin and Yang, Yuer and Xin, Xiangjun and Song, Chang},
  journal={IEEE Transactions on Cloud Computing},
  year={2024},
  publisher={IEEE}
}
```

Thank you for your citations. 

#pragma once

#ifdef __BASE_CLASS_EXPORT__
#define __BASE_CLASS__ __declspec(dllexport)
#else
#define __BASE_CLASS__ __declspec(dllexport)
#endif

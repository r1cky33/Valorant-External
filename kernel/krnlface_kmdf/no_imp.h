//Setup Import Defines
#define ImpSet(a) a##Fn = (a##Def)utils::EPtr(krnlutils::km_get_proc_address(kbase, E(#a)));
#define ImpDef(a) using a##Def = decltype(&a); a##Def a##Fn = nullptr;
#define ImpCall(a, ...) ((a##Def)utils::EPtr(a##Fn))(__VA_ARGS__)
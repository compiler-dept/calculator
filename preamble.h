void *ParseAlloc(void *(*allocProc) (size_t));
void Parse(void *, int, const char *);
void ParseFree(void *, void (*freeProc) (void *));

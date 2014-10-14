void *ParseAlloc(void *(*allocProc) (size_t));
void Parse(void *, int, const char *, char *error);
void ParseFree(void *, void (*freeProc) (void *));

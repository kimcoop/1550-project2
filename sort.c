int intcmp( const void *n1, const void *n2 ) {
  const int num1 = *(const int *) n1;
  const int num2 = *(const int *) n2;
  return (num1 < num2) ? -1 : (num1 > num2);
}

int structCmpFirst( const void *struct_a, const void *struct_b ) { 
  // struct_a and struct_b are pointers to an element of the array, aka pointers to pointers to structs. must cast it, then dereference it
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return strcmp(  a->FirstName, b->FirstName ); 
} //  structCmpFirst

int structCmpLast( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return strcmp(  a->LastName, b->LastName ); 
} //  structCmpLast

int structCmpSsn( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return intcmp(  &a->ssn, &b->ssn );
} // structCmp structCmpSsn

int structCmpIncome( const void *struct_a, const void *struct_b ) { 
  MyRecord *a = *( MyRecord ** ) struct_a; MyRecord *b = *( MyRecord ** ) struct_b;
  return intcmp(  &a->income, &b->income );
} // structCmp structCmpIncome

void sortRecords( MyRecord** records, int numRecs, int attr ) {

  if ( attr == KEY_LASTNAME )
    qsort( records, numRecs, sizeof( MyRecord* ), structCmpLast );
  else if ( attr == KEY_FIRSTNAME  )
    qsort( records, numRecs, sizeof( MyRecord* ), structCmpFirst );
  else if ( attr == KEY_SSN )
    qsort( records, numRecs, sizeof( MyRecord ), structCmpSsn );
  else
    qsort( records, numRecs, sizeof( MyRecord ), structCmpIncome );

} // sortRecords












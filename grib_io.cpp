#include <iostream>
#include <eccodes.h>
#include <assert.h>

void read_grib_file(double gh[], size_t len){
    FILE * f = fopen("gfs.t18z.pgrb2.0p25.anl","r");
    if(f == NULL){
        fprintf(stderr, "Error: opening grib file\n");
        return;
    }
    int error;
    char value[5000];
    size_t vlen=5000;
    grib_handle * gid;
    while((gid = codes_grib_handle_new_from_file(NULL, f, &error))!=NULL){;
        if(error != CODES_SUCCESS){
            fprintf(stderr, "Error: opening grib handle\n");
            fclose(f);
            return;
        }

        bzero(value,5000);
        if( codes_is_defined(gid, "shortName") == 0 ){
            printf("No shortName for this\n");
        }
        else{
            vlen=5000;
            if(codes_get_string(gid, "shortName",value,&vlen) != 0){
                printf("Error reading\n");
            }
            else{
                if(strcmp("gh", value) == 0){
                    long level;
                    codes_get_long(gid, "level", &level);
                    if(level == 500){
                        assert(codes_is_defined(gid, "values"));
                        if(codes_get_double_array(gid, "values", gh, &len)){
                            perror("fdsa");
                        }
                        codes_handle_delete(gid);
                        return;
                    }
                }
            }
        }
        codes_handle_delete(gid);
    }
}

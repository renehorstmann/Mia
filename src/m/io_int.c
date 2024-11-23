#include "m/io/ivecn.h"
#include "m/sca/int.h"
#include "o/OObjRoot.h"
#include "o/OJson.h"
#include "o/terminalcolor.h"
#include <locale.h>


osize ivecn_buffer(char *str, osize size, const int *v, int n, bool colored, bool typed)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(ivec%i) ", n);
    } else {
        used += snprintf(str, size, "(int[%i]) ", n);
    }
    if(used>=size) {
        str = NULL;
    }

    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_CYAN);
        if(used>=size) {
            str = NULL;
        }
    }

    if (typed) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{{ ");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{  ");
    }
    if(used>=size) {
        str = NULL;
    }

    for (int i = 0; i < n; i++) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "%" mi_PRI "%s", v[i], i < n - 1 ? ", " : "");
        if (used >= size) {
            str = NULL;
        }
    }

    if (typed && n >= 2 && n <= 4) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, " }}");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, "  }");
    }
    if (used >= size) {
        str = NULL;
    }


    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_RESET);
    }

    return used;
}

char *ivecn_str(oobj parent, const int *v, int n, bool colored, bool typed)
{
    osize len = ivecn_buffer(NULL, 0, v, n, colored, typed);
    len += 1;
    char *str = o_new(parent, char, len);
    ivecn_buffer(str, len, v, n, colored, typed);
    return str;
}

osize imatn_buffer(char *str, osize size, const int *m, int n, bool colored, bool typed, bool multiline)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(imat%i) ", n);
    } else {
        used += snprintf(str, size, "(imat[%i]) ", n);
    }
    if(used>=size) {
        str = NULL;
    }

    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_CYAN);
        if(used>=size) {
            str = NULL;
        }
    }

    if(multiline) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "\n");
        if(used>=size) {
            str = NULL;
        }
    }

    if (typed) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{{ ");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{  ");
    }
    if(used>=size) {
        str = NULL;
    }

    for (int col = 0; col < n; col++) {
        for(int row=0; row < n; row++) {
            used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                             "%" mi_PRI "%s", m[col * n + row], row < n - 1 ? ", " : "");
            if (used >= size) {
                str = NULL;
            }
        }
        if(col < n-1) {
            if(multiline) {
                used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                                 ",\n   ");
            } else {
                used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                                 ", /**/ ");
            }
            if(used>=size) {
                str = NULL;
            }
        }
    }

    if (typed && n >= 2 && n <= 4) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, " }}");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, "  }");
    }
    if (used >= size) {
        str = NULL;
    }


    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_RESET);
    }

    return used;
}

char *imatn_str(oobj parent, const int *m, int n, bool colored, bool typed, bool multiline)
{
    osize len = imatn_buffer(NULL, 0, m, n, colored, typed, multiline);
    len += 1;
    char *str = o_new(parent, char, len);
    imatn_buffer(str, len, m, n, colored, typed, multiline);
    return str;
}

osize ivecn_print(const int *v, int n)
{
    oobj root = OObjRoot_new_heap();
    char *str = ivecn_str(root, v, n, true, true);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize ivecn_println(const int *v, int n)
{
    osize written = ivecn_print(v, n);
    written += printf("\n");
    return written;
}

osize imatn_print(const int *m, int n, bool multiline)
{
    oobj root = OObjRoot_new_heap();
    char *str = imatn_str(root, m, n, true, true, multiline);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize imatn_println(const int *m, int n, bool multiline)
{
    osize written = imatn_print(m, n, multiline);
    written += printf("\n");
    return written;
}

bool ivecn_json_read(int *out_v, int n, oobj json_array)
{
    OJson **list = OJson_list(json_array, NULL);
    int idx = 0;
    for(OJson **it=list; *it && idx<n; it++) {
        double *num = OJson_number(*it);
        if(num) {
            out_v[idx++] = (int) *num;
        }
    }
    o_free(json_array, list);
    return idx==n;
}

oobj ivecn_json_new(oobj parent, const char *name, const int *v, int n)
{
    OJson *array = OJson_new_array(parent, name);
    OJson_packed_set(array, true);
    for(int i=0; i<n; i++) {
        char buf[32];
        snprintf(buf, sizeof buf, "v%i", i);
        OJson_new_number(array, buf, (double) v[i]);
    }
    return array;
}

bool imatn_json_read(int *out_m, int n, oobj json_array)
{
    OJson **list = OJson_list(json_array, NULL);
    int col_idx = 0;
    int idx = 0;
    for(OJson **it=list; *it && col_idx < n; it++) {
        if(OJson_type(*it) != OJson_TYPE_ARRAY) {
            continue;
        }

        OJson **ivec = OJson_list(*it, NULL);
        int row_idx = 0;
        for(OJson **ivec_it=ivec; *ivec_it && row_idx < n; ivec_it++) {
            double *num = OJson_number(*ivec_it);
            if(num) {
                out_m[idx++] = (int) *num;
                row_idx++;
            }
        }
        o_free(*it, ivec);
        if(row_idx < n) {
            break;
        }
        col_idx++;
    }
    o_free(json_array, list);
    return idx==n*n;
}

oobj imatn_json_new(oobj parent, const char *name, const int *m, int n)
{
    OJson *array = OJson_new_array(parent, name);
    for(int col=0; col<n; col++) {
        char buf[32];
        snprintf(buf, sizeof buf, "col[%i]", col);
        OJson *ivec = OJson_new_array(array, name);
        OJson_packed_set(ivec, true);
        for(int row=0; row<n; row++) {
            int idx = col*n + row;
            snprintf(buf, sizeof buf, "v%i", row);
            OJson_new_number(ivec, buf, (double) m[idx]);
        }
    }
    return array;
}

#ifndef ERR_H
#define ERR_H 

/* Error stuff for s0 library */

typedef enum {
    s0_SUCCESS = 0,
    /* Not implemented error */
    s0_err_NOTIMP,
    /* Bad type errror */
    s0_err_BADTYPE,
    /* Bad index */
    s0_err_BADINDEX,
    /* Bad channel value */
    s0_err_BADCHAN,
    /* Bad frames value */
    s0_err_BADFRAMES,
    /* Couldn't allocate memory */
    s0_err_BADALLOC,
    /* Bad channel combination */
    s0_err_BADCHANCOMB
    /* Bad frame combination */
    s0_err_BADFRAMECOMB
} s0_err_t;

#endif /* ERR_H */

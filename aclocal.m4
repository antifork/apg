dnl
dnl  $Id$
dnl  aclocal macro: apg
dnl
dnl Copyright (c) 1995, 1996, 1997, 1998
dnl     The Regents of the University of California.  All rights reserved.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that: (1) source code distributions
dnl retain the above copyright notice and this paragraph in its entirety, (2)
dnl distributions including binary code include the above copyright notice and
dnl this paragraph in its entirety in the documentation or other materials
dnl provided with the distribution, and (3) all advertising materials mentioning
dnl features or use of this software display the following acknowledgement:
dnl ``This product includes software developed by the University of California,
dnl Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
dnl the University nor the names of its contributors may be used to endorse
dnl or promote products derived from this software without specific prior
dnl written permission.
dnl THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
dnl WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
dnl MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
dnl
dnl LBL autoconf macros
dnl
dnl from: libpcap-0.4
dnl
dnl Check for flex, default to lex
dnl Require flex 2.4 or higher
dnl Check for bison, default to yacc
dnl Default to lex/yacc if both flex and bison are not available
dnl Define the yy prefix string if using flex and bison
dnl
dnl usage:
dnl
dnl     AC_LBL_LEX_AND_YACC(lex, yacc, yyprefix)
dnl
dnl results:
dnl
dnl     $1 (lex set)
dnl     $2 (yacc appended)
dnl     $3 (optional flex and bison -P prefix)
dnl
dnl

AC_DEFUN(AC_LBL_LEX_AND_YACC,
    [AC_ARG_WITH(flex, [  --without-flex          don't use flex])
    AC_ARG_WITH(bison, [  --without-bison         don't use bison])
    if test "$with_flex" = no ; then
            $1=lex
    else
            AC_CHECK_PROGS($1, flex, lex)
    fi
    if test "$$1" = flex ; then
            # The -V flag was added in 2.4
            AC_MSG_CHECKING(for flex 2.4 or higher)
            AC_CACHE_VAL(ac_cv_lbl_flex_v24,
                if flex -V >/dev/null 2>&1; then
                        ac_cv_lbl_flex_v24=yes
                else
                        ac_cv_lbl_flex_v24=no
                fi)
            AC_MSG_RESULT($ac_cv_lbl_flex_v24)
            if test $ac_cv_lbl_flex_v24 = no ; then
                    s="2.4 or higher required"
                    AC_MSG_WARN(ignoring obsolete flex executable ($s))
                    $1=lex
            fi
    fi
    if test "$with_bison" = no ; then
            $2=yacc
    else
            AC_CHECK_PROGS($2, bison, yacc)
    fi
    if test "$$2" = bison ; then
            $2="$$2 -y"
    fi
    if test "$$1" != lex -a "$$2" = yacc -o "$$1" = lex -a "$$2" != yacc ; then
            AC_MSG_WARN(don't have both flex and bison; reverting to lex/yacc)
            $1=lex
            $2=yacc
    fi
    if test "$$1" = flex -a -n "$3" ; then
            $1="$$1 -P$3"
            $2="$$2 -p $3"
    fi])


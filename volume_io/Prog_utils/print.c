#include  <internal_volume_io.h>
#include  <stdarg.h>

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/minc/volume_io/Prog_utils/print.c,v 1.7 1995-04-28 18:32:54 david Exp $";
#endif

private  void  (*saved_print_function) ( char [] );
private  void  (*print_function) ( char [] );

/* ----------------------------- MNI Header -----------------------------------
@NAME       : set_print_function
@INPUT      : function
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Sets the output function.  If you use the function print()
              everywhere, in place of printf, then by default it uses
              printf to send output to stdout.  However, you can call
              the set_print_function() to tell it to use a different output
              function, e.g. output to a GL or X window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  set_print_function( void (*function) ( char [] ) )
{
    print_function = function;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : push_print_function
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Save the current print function, so, for instance, you can
              print to stdout temporarily.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  push_print_function()
{
    saved_print_function = print_function;
    print_function = 0;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : pop_print_function
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Restore the print function.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  pop_print_function()
{
    print_function = saved_print_function;
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : print
@INPUT      : exactly same arguments as printf
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: prints the arguments to a temporary string buffer, then either
              printf's the or calls the user installed function to output
              the string.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

/* VARARGS */
public  void  print( char format[], ... )
{
    va_list  ap;
    char     print_buffer[10000];

    va_start( ap, format );
    (void) vsprintf( print_buffer, format, ap );
    va_end( ap );

    if( print_function == NULL )
        (void) printf( "%s", print_buffer );
    else
        (*print_function) ( print_buffer );
}

/* VARARGS */
public  void  print_error( char format[], ... )
{
    va_list  ap;
    char     print_buffer[10000];

    va_start( ap, format );
    (void) vsprintf( print_buffer, format, ap );
    va_end( ap );

    (void) fprintf( stderr, "%s", print_buffer );
}


/* ----------------------------- MNI Header -----------------------------------
@NAME       : handle_internal_error
@INPUT      : str
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Prints the error string and tries to get users permission to
              abort.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void   handle_internal_error( char  str[] )
{
    push_print_function();
    print( "Internal error:  %s\n", str );
    abort_if_allowed();
    pop_print_function();
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : abort_if_allowed
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Checks if the user wants to abort.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    :                      David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  abort_if_allowed( void )
{
    char  ch;

    if( !ENV_EXISTS( "NO_ABORT" ) )
    {
        set_print_function( NULL );

        print( "Do you wish to abort (y/n): " );
        do
        {
            ch = getchar();
        }
        while( ch != 'y' && ch != 'n' );

        while( getchar() != '\n' )
        {
        }

        if( ch == 'y' )
        {
            abort();
        }
    }
}

public  void  begin_error()
{
    push_print_function();
    set_print_function( NULL );
}

public  void  end_error()
{
    pop_print_function();
}

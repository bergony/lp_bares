#include <iostream>
#include <iostream>  // cout, endl
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error

#include "../include/infix_to_postfix.h"

std::vector< Token > infix_to_postfix::get_m_token_list( void )
{
    return m_token_list_post;
}

void infix_to_postfix::set_m_token_list( const std::vector< Token > & c_expr )
{
    m_token_list_post = c_expr;
}

bool infix_to_postfix::a_number( Token::token_type c)
{
    return c == Token::token_type::OPERAND;
}

bool infix_to_postfix::a_operator( Token::token_type c )
{
    if(c == Token::token_type::OPERATOR || c == Token::token_type::POWER_OPERATOR)
    {
        return true;
    }
    return false;
}

bool infix_to_postfix::close_scope( Token::token_type c )
{
    return c == Token::token_type::CLOSE_SCOPE;
}
bool infix_to_postfix::open_scope( Token::token_type c )
{
    return c == Token::token_type::OPEN_SCOPE;
}

bool infix_to_postfix::power_operator( std::string c )
{
    return c[0] == '^';
}


int infix_to_postfix::get_precedence( std::string op )
{
    int cont{0};
    while( op.begin()+cont != op.end())
    {
        if(op[cont] != ' ')
        {
            break; 
        }
    }
    switch( op[cont] )
    {
        case '^' : return 3;
        case '*' :
        case '/' :
        case '%' : return 2;
        case '+' :
        case '-' : return 1;
        case ')' : return 0;
        case '(' : return 0;
        default  : return -1;

    }
}

bool infix_to_postfix::precedence( std::string op1, std::string op2 )
{
    int cast_op1 = get_precedence( op1 );
    int cast_op2 = get_precedence( op2 );

    if( cast_op1 > cast_op2 )
    {
        return true;
    }
    else if( cast_op1 < cast_op2)
    {
        return false;
    }
    else
    {
        if( power_operator( op1 ) ) return false;

        return true;
    }
}

std::vector< Token > infix_to_postfix::convert( void )
{
    std::vector< Token > postfix;
    std::stack< Token > c;

    for( auto i : m_token_list_post )
    {   
        if( a_number( i.type ) )
            postfix.emplace_back( i );
        else if( open_scope( i.type ) )
            c.push( i );
        else if( close_scope( i.type ) )
        {
            while( !open_scope( c.top().type ) )
            {
                postfix.emplace_back( c.top() );
                c.pop();
            }
            c.pop();
        }
        else if( a_operator(i.type) )
        {
            while( !c.empty() && precedence( c.top().value, i.value) )
            {  
                postfix.emplace_back( c.top() );
                c.pop();
            }
            c.push( i );
        }
        else
        {

        }
    }

    while( !c.empty() )
    {
        postfix.emplace_back( c.top() );
        c.pop();
    }
    swap(m_token_list_post , postfix );

    return m_token_list_post;
}
int infix_to_postfix::str_to_int( std::string c )
{
    int numb_int = stoll( c );

    return numb_int;
}
int infix_to_postfix::math_op( int numb1, int numb2, std::string c )
{
    switch( c[0] )
    {
        case '^':  return pow( numb1,numb2 );
        case '*':  return numb1*numb2;
        case '/':{  if ( numb2 == 0 )
                     {
                         std::cout << "divisao por zer";
                         return 0;
                     }
                     else
                         return numb1/numb2;
                 }
        case '%':{  if ( numb2 == 0 ) 
                     {
                         std::cout << "divisao por zer";
                         return 0;
                     }
                     else
                         return numb1%numb2;;
                 }
        case '+': return numb2+numb1;
        case '-': return numb1-numb2;
        default: throw std::runtime_error( "undefined operator" );

    }
}
int infix_to_postfix::evaluate( void )
{
    std::stack< int > c;
    for( auto i : m_token_list_post )
    {
        if( a_number(i.type) )
        {
            c.push( str_to_int( i.value ) );
        }
        else if( a_operator( i.type ) )
        {
            auto op2 = c.top(); 
            c.pop();
            auto op1 = c.top(); 
            c.pop();
            //std::cout << op1 << " " << " " << op2 << " \n"; 
            auto value = math_op( op1, op2, i.value );
            c.push( value );

        }else assert( false );
    }
    if(c.top() < -32768 || c.top() > 32768)
    {
        std::cout << " Numeric overflow err"; 
        return 0;
    }
    return c.top(); 
}

#include <algorithm>
#include <string>
#include <iterator>


#include "../include/parser.h"

Parser::symbol_type Parser::get_symbol( char x) const
{
    switch( x ){
        case '+': return symbol_type::ST_SUM;
        case '-': return symbol_type::ST_DIF;
        case '*': return symbol_type::ST_PROD;
        case '/': return symbol_type::ST_QUOT;
        case '%': return symbol_type::ST_MOD;
        case '^': return symbol_type::ST_POWER;
        case '(': return symbol_type::ST_OPEN_SCOPE;
        case ')': return symbol_type::ST_CLOSE_SCOPE;
        case ' ': return symbol_type::ST_SPACE;
        case   9: return symbol_type::ST_TAB;
        case '0': return symbol_type::ST_ZERO;
        case '1': 
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': return symbol_type::ST_DIGIT;
        case '\0': return symbol_type::ST_END;
    }
    return symbol_type::ST_INVALID;
}

void Parser::next_symbol( void )
{ 
    m_point_expr++;
}
bool Parser::end_string( void ) const
{
    return m_point_expr == m_expr.end();
}
void Parser::next_space( void ) 
{
    while( !end_string() && std::isspace( *m_point_expr ) )
    {
        next_symbol();
    }
}
bool Parser::accept( symbol_type x )
{   

    if( !end_string() && get_symbol( *m_point_expr ) == x )
    {   
        if( get_symbol( *m_point_expr) == symbol_type::ST_QUOT ||
               get_symbol( *m_point_expr) == symbol_type::ST_MOD )
        {   
            m_point_expr++;
            next_space();
            if(get_symbol( *(m_point_expr) ) == symbol_type::ST_ZERO ) 
            {
                m_result = ResultType( ResultType::DIV_ZERO ,
                std::distance( m_expr.begin(), m_point_expr ));
                return false;
            }
            m_point_expr--;
        }

        if( get_symbol( *m_point_expr) == symbol_type::ST_DIF && 
            get_symbol( *m_point_expr+1) == symbol_type::ST_DIF )
                m_point_expr++;

       //std::cout << "Accept " <<*m_point_expr << "\n";
        next_symbol();
        return true;
    }
    //std::cout << " Negate " << *m_point_expr << "\n";
    return false;
}
/* ============ Gramática ===========*/
bool Parser::expression( void )
{
    //std::cout << " IN expresion\n";
    term();
    while( m_result.type == ResultType::OK )
    {
        next_space();
        if( accept( Parser::symbol_type::ST_DIF ))
        {
            m_token_list.emplace_back( Token( "-", Token::token_type::OPERATOR ) );
        }
        else if( accept( Parser::symbol_type::ST_SUM ) )
        {
            m_token_list.emplace_back( Token( "+", Token::token_type::OPERATOR ));
        }
        else if( accept( Parser::symbol_type::ST_PROD ) )
        {
            m_token_list.emplace_back( Token( "*", Token::token_type::OPERATOR )); 
        }
        else if( accept( Parser::symbol_type::ST_QUOT) )
        {
            m_token_list.emplace_back( Token( "/", Token::token_type::OPERATOR )); 
        }
        else if( accept( Parser::symbol_type::ST_MOD ) )
        {
            m_token_list.emplace_back( Token( "%", Token::token_type::OPERATOR )); 
        }
        else if( accept( Parser::symbol_type::ST_POWER ) )
        {
            m_token_list.emplace_back( Token( "^", Token::token_type::POWER_OPERATOR )); 
        }
        else break;

        if( !term()  && m_result.type == ResultType::INT_OUT_RANGE )
            m_result.type = ResultType::INT_OUT_RANGE;
    }

    return m_result.type == ResultType::OK;
}

bool Parser::term()
{
    next_space();
    int numb_minus{0};
    auto begin_point_token( m_point_expr );
    //std::cout << "IN term\n";
    if( accept( Parser::symbol_type::ST_OPEN_SCOPE ) )
    {
        m_token_list.emplace_back( Token( "(", Token::token_type::OPEN_SCOPE ) ); 
        next_space();
        if( end_string() )
        {
            m_result = ResultType( ResultType::INCOMPLETE_EXPRE, 
                std::distance( m_expr.begin() , m_expr.end() ) );
            }else 
            {
            if( expression() )
            next_space();
            if(  !accept( Parser::symbol_type::ST_CLOSE_SCOPE ) )
            {
                m_result = ResultType( ResultType::MISS_CLOSE_SCOPE,
                        std::distance( m_expr.begin(), m_point_expr ) );
            }else
            {
                m_token_list.emplace_back(  Token( ")", Token::token_type::CLOSE_SCOPE ) );
            }

        }

    }
    else if( integer() )
    {
        std::string numb_int_str;
        std::copy( begin_point_token , m_point_expr ,std::back_inserter( numb_int_str ) );
        int numb_int;
        for(unsigned int x{0}; x < numb_int_str.size(); x++ )
        {
            if( numb_int_str.at(x) == '-')
                    numb_minus++;
        }

        int cont{0};
        if(numb_minus%2 == 0)
        {
            while( cont < numb_minus)
            {
               numb_int_str.at(cont) = ' ';
               cont++;
            }
         }else
         {
            numb_minus--;
            while( cont < numb_minus)
            {
               numb_int_str.at(cont) = ' ';
               cont++;
            }
         }
        //std::cout << numb_int_str << "\n";
        try{ numb_int = stoll( numb_int_str ); }
        catch( const std::invalid_argument & e )
        {
            //std::cout << *m_point_ex << numb_int_str[0] << "\n";
            
            if( *begin_point_token == '-')
            {
                m_result = ResultType( ResultType::IMCOMPLETE_INT,
                        std::distance( m_expr.begin(), begin_point_token ) );
            }else if ( *begin_point_token == '\0' ){
                m_result = ResultType( ResultType::MISS_TERM,
                        std::distance( m_expr.begin(), begin_point_token ) );
            }else
            {
            m_result = ResultType( ResultType::IMCOMPLETE_INT ,
                    std::distance( m_expr.begin(), begin_point_token ) );
            }
            return false;
        }
        catch( const std::out_of_range & e )
        {
            m_result = ResultType( ResultType::INT_OUT_RANGE ,
                    std::distance( m_expr.begin(), begin_point_token ) );
            return false;
        }
        if(numb_int > -32768 && numb_int < 32768 )
        {
            m_token_list.emplace_back( Token( numb_int_str , Token::token_type::OPERAND ) );
        }else
        {
            m_result = ResultType( ResultType::INT_OUT_RANGE ,
                    std::distance( m_expr.begin() , begin_point_token ) );
        }

    }else
    {
       // std::cout << "oi \n";
       // std::cout << "<IMCOMPLETE_INT\n";
        m_result = ResultType( ResultType::IMCOMPLETE_INT ,
                      std::distance( m_expr.begin() , begin_point_token ) );
    }

    return m_result.type == ResultType::OK;
}
bool Parser::integer( void )
{
    //std::cout<< "INT IN\n";
    if( accept( symbol_type::ST_ZERO ))
        return true;

    while ( accept( symbol_type::ST_DIF ))
    {
        //std::cout << *m_point_expr << "\n";
        //m_point_expr++;
    }
    //std::cout << *m_point_expr << "\n";
    if( accept( symbol_type::ST_OPEN_SCOPE) || accept( symbol_type::ST_CLOSE_SCOPE) )
        return false;

    return natural();
}
bool Parser::natural( void )
{
    //std::cout << "nataural sim \n";
    if( digit_zero()  )
        return false;

    while( digit() )
    { }

    return true;

}

bool Parser::digit_zero( void )
{
    //std::cout << "zero sim \n";
    return accept( symbol_type::ST_ZERO );

}

bool Parser::digit( void )
{
   // std::cout << "digit sim \n";
    return( accept( symbol_type::ST_DIGIT ) || digit_zero() ) ? true : false;
}

Parser::ResultType Parser::parser( std::string expre_ )
{

    m_expr = expre_;
    m_point_expr = m_expr.begin();
    m_result = ResultType( ResultType::OK );

    m_token_list.clear();

    next_space();
    if( end_string() )
    {
        m_result = ResultType( ResultType::INCOMPLETE_EXPRE, 
                std::distance( m_expr.begin() , m_expr.end() ) );
    }else{
       // std::cout << "IN parser\n"; 
        if( expression() )
        {
            next_space();
            if( !end_string() )
            {
                m_result = ResultType( ResultType::WRONG_SYMBOL,
                        std::distance( m_expr.begin(), m_point_expr ) );
            }
        }

    }
    return m_result;
}
std::vector< Token > Parser::get_tokens( void ) const
{
    return m_token_list;
}


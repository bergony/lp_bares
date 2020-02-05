#ifndef _INFIX_TO_POSTFIX_
#define _INFIX_TO_POSTFIX_

#include "parser.h"

#include <string>
#include <stack>
    /// <b>Class</b> infix_to_postfix <i>Recive to token list infix and change to  postfix.</i>
class infix_to_postfix{
    private:
        std::vector< Token > m_token_list_post; //!< List of token post

    public:

        //! Is a get method to see list of Token.
        /*!
        \return std::vector< Token > The list of the Token.
        */
        std::vector< Token > get_m_token_list( void );
        //! Is a set method to change the list Token.
        /*!
        \param std::vector< Token > List of token to be save.
        */
        void set_m_token_list( const std::vector< Token > & c_expr );
        //! Check if the Token::token_type is a OPERAND.
        /*!
        \param Token::token_type Token to be check.
        \return True if the Token::type is a OPERAND.
        \return False Otherwise.
        */
        bool a_number( Token::token_type c );
        //! Check if the Token::token_type is a OPERATOR.
        /*!
        \param Token::token_type Token to be check.
        \return True if the Token::token_type is a OPERATOR.
        \return False Otherwise.
        */
        bool a_operator( Token::token_type c );
        //! Check is the Token::token_type is a CLOSE_SCOPE.
        /*!
        \param Token::token_type Token to be check.
        \return True if the Token::token_type is a CLOSE_SCOPE.
        \return False Otherwise.
        */
        bool close_scope( Token::token_type c);
        //! Check is the Token::token_type is a OPEN_SCOPE.
        /*!
        \param Token::token_type Token to be check.
        \return True if the Token::token_type is a OPEN_SCOPE.
        \return False Otherwise.
        */
        bool open_scope( Token::token_type c);
        //! Check is the Token::value is a "^".
        /*!
        \param std::string  string to be check.
        \return True if the Token::value are equal to "^".
        \return False Otherwise.
        */
        bool power_operator( std::string c );
        //! Calculates the value Param1 Param3 param2
        /*!
        \param int numb1 first number of operation
        \param int numb2 second number of operation.
        \param std::string "c" operator of operation.
        \return int result of operation.
        */
        int math_op( int numb1, int numb2, std::string c );
        //! Calculates the expression save on m_token_list_post
        /*!
        \return int result of operation with Token.
        */
        int evaluate( void );
        //! Convert string numbers to a integer !
        /*!
        \param std::string "c" string to be convert
        \return int integer convert.
        */
        int str_to_int( std::string c );
        //! Precedence of Operator
        /*!
        \param std::string "OP" Operator to get precedence
        \return int 3 = ( '^' ).
        \return int 2 = ( '*', '/', '%' ).
        \return int 1 = ( '+', '-' ).
        \return int 0 = ( '(', ).
        */
        int get_precedence( std::string op );
        //! Looks with operator have more precedence. 
        /*!
        \param std::string "op1" Operator first
        \param std::string "op2" Operator second
        \return True Op1 > Op2 or op1 == op2 
                            and not a POWER_OPERATOR.
        \return False Op1 < OP2 or op1 == op2 
                            and is a POWER_OPERATOR.
        */
        bool precedence( std::string op1, std::string op2 );
        //! Convert the m_token_list_post to postfix.
        /*!
        \return std::vector< Token > The New m_token_list_post Now postfix.
        \return False Otherwise.
        */
        std::vector< Token > convert( void );

};


#endif

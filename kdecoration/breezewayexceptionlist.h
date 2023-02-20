#ifndef breezewayexceptionlist_h
#define breezewayexceptionlist_h

//////////////////////////////////////////////////////////////////////////////
// breezewayexceptionlist.h
// window decoration exceptions
// -------------------
//
// SPDX-FileCopyrightText: 2009 Hugo Pereira Da Costa <hugo.pereira@free.fr>
//
// SPDX-License-Identifier: MIT
//////////////////////////////////////////////////////////////////////////////

#include "breezewaysettings.h"
#include "breezeway.h"

#include <KSharedConfig>

namespace Breezeway
{

    //! breezeway exceptions list
    class ExceptionList
    {

        public:

        //! constructor from list
        explicit ExceptionList( const InternalSettingsList& exceptions = InternalSettingsList() ):
            _exceptions( exceptions )
        {}

        //! exceptions
        const InternalSettingsList& get( void ) const
        { return _exceptions; }

        //! read from KConfig
        void readConfig( KSharedConfig::Ptr );

        //! write to kconfig
        void writeConfig( KSharedConfig::Ptr );

        protected:

        //! generate exception group name for given exception index
        static QString exceptionGroupName( int index );

        //! read configuration
        static void readConfig( KCoreConfigSkeleton*, KConfig*, const QString& );

        //! write configuration
        static void writeConfig( KCoreConfigSkeleton*, KConfig*, const QString& );

        private:

        //! exceptions
        InternalSettingsList _exceptions;

    };

}

#endif

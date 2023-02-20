#ifndef breezewayconfigwidget_h
#define breezewayconfigwidget_h
//////////////////////////////////////////////////////////////////////////////
// breezewayconfigurationui.h
// -------------------
//
// SPDX-FileCopyrightText: 2009 Hugo Pereira Da Costa <hugo.pereira@free.fr>
//
// SPDX-License-Identifier: MIT
//////////////////////////////////////////////////////////////////////////////

#include "ui_breezewayconfigurationui.h"
#include "breezewayexceptionlistwidget.h"
#include "breezewaysettings.h"
#include "breezeway.h"

#include <KCModule>
#include <KSharedConfig>

#include <QWidget>
#include <QSharedPointer>

namespace Breezeway
{

    //_____________________________________________
    class ConfigWidget: public KCModule
    {

        Q_OBJECT

        public:

        //* constructor
        explicit ConfigWidget( QWidget*, const QVariantList& );

        //* destructor
        virtual ~ConfigWidget() = default;

        //* default
        void defaults() override;

        //* load configuration
        void load() override;

        //* save configuration
        void save() override;

        protected Q_SLOTS:

        //* update changed state
        virtual void updateChanged();

        protected:

        //* set changed state
        void setChanged( bool );

        private:

        //* ui
        Ui_BreezewayConfigurationUI m_ui;

        //* kconfiguration object
        KSharedConfig::Ptr m_configuration;

        //* internal exception
        InternalSettingsPtr m_internalSettings;

        //* changed state
        bool m_changed;

    };

}

#endif

/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 * SPDX-FileCopyrightText: 2014 Hugo Pereira Da Costa <hugo.pereira@free.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */
#include "breezewaybutton.h"

#include <KDecoration2/DecoratedClient>
#include <KColorUtils>
#include <KIconLoader>

#include <QPainter>
#include <QVariantAnimation>
#include <QPainterPath>

namespace Breezeway
{

    using KDecoration2::ColorRole;
    using KDecoration2::ColorGroup;
    using KDecoration2::DecorationButtonType;


    //__________________________________________________________________
    Button::Button(DecorationButtonType type, Decoration* decoration, QObject* parent)
        : DecorationButton(type, decoration, parent)
        , m_animation( new QVariantAnimation( this ) )
    {

        // setup animation
        // It is important start and end value are of the same type, hence 0.0 and not just 0
        m_animation->setStartValue( 0.0 );
        m_animation->setEndValue( 1.0 );
        m_animation->setEasingCurve( QEasingCurve::InOutQuad );
        connect(m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
            setOpacity(value.toReal());
        });

        // setup default geometry
        const int height = decoration->buttonHeight();
        setGeometry(QRect(0, 0, height, height));
        setIconSize(QSize( height, height ));

        // connections
        connect(decoration->client().toStrongRef().data(), SIGNAL(iconChanged(QIcon)), this, SLOT(update()));
        connect(decoration->settings().data(), &KDecoration2::DecorationSettings::reconfigured, this, &Button::reconfigure);
        connect( this, &KDecoration2::DecorationButton::hoveredChanged, this, &Button::updateAnimationState );

        reconfigure();

    }

    //__________________________________________________________________
    Button::Button(QObject *parent, const QVariantList &args)
        : Button(args.at(0).value<DecorationButtonType>(), args.at(1).value<Decoration*>(), parent)
    {
        m_flag = FlagStandalone;
        //! icon size must return to !valid because it was altered from the default constructor,
        //! in Standalone mode the button is not using the decoration metrics but its geometry
        m_iconSize = QSize(-1, -1);
    }
            
    //__________________________________________________________________
    Button *Button::create(DecorationButtonType type, KDecoration2::Decoration *decoration, QObject *parent)
    {
        if (auto d = qobject_cast<Decoration*>(decoration))
        {
            Button *b = new Button(type, d, parent);
            const auto c = d->client().toStrongRef();
            switch( type )
            {

                case DecorationButtonType::Close:
                b->setVisible( c->isCloseable() );
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::closeableChanged, b, &Breezeway::Button::setVisible );
                break;

                case DecorationButtonType::Maximize:
                b->setVisible( c->isMaximizeable() );
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::maximizeableChanged, b, &Breezeway::Button::setVisible );
                break;

                case DecorationButtonType::Minimize:
                b->setVisible( c->isMinimizeable() );
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::minimizeableChanged, b, &Breezeway::Button::setVisible );
                break;

                case DecorationButtonType::ContextHelp:
                b->setVisible( c->providesContextHelp() );
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::providesContextHelpChanged, b, &Breezeway::Button::setVisible );
                break;

                case DecorationButtonType::Shade:
                b->setVisible( c->isShadeable() );
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::shadeableChanged, b, &Breezeway::Button::setVisible );
                break;

                case DecorationButtonType::Menu:
                QObject::connect(c.data(), &KDecoration2::DecoratedClient::iconChanged, b, [b]() { b->update(); });
                break;

                default: break;

            }

            return b;
        }

        return nullptr;

    }

    //__________________________________________________________________
    void Button::paint(QPainter *painter, const QRect &repaintRegion)
    {
        Q_UNUSED(repaintRegion)

        if (!decoration()) return;

        painter->save();

        // translate from offset
        if( m_flag == FlagFirstInList ) painter->translate( m_offset );
        else painter->translate( 0, m_offset.y() );

        if( !m_iconSize.isValid() || isStandAlone() ) m_iconSize = geometry().size().toSize();

        // menu button
        if (type() == DecorationButtonType::Menu)
        {

            const QRectF iconRect( geometry().topLeft(), m_iconSize );
            const auto c = decoration()->client().toStrongRef();
            if (auto deco =  qobject_cast<Decoration*>(decoration())) {
                const QPalette activePalette = KIconLoader::global()->customPalette();
                QPalette palette = c->palette();
                palette.setColor(QPalette::WindowText, deco->fontColor());
                KIconLoader::global()->setCustomPalette(palette);
                c->icon().paint(painter, iconRect.toRect());
                if (activePalette == QPalette()) {
                    KIconLoader::global()->resetPalette();
                }    else {
                    KIconLoader::global()->setCustomPalette(palette);
                }
            } else {
                c->icon().paint(painter, iconRect.toRect());
            }

        } else {

            drawIcon( painter );

        }

        painter->restore();

    }

    //__________________________________________________________________
    void Button::drawIcon( QPainter *painter ) const
    {

        painter->setRenderHints( QPainter::Antialiasing );

        /*
        scale painter so that its window matches QRect( -1, -1, 20, 20 )
        this makes all further rendering and scaling simpler
        all further rendering is preformed inside QRect( 0, 0, 18, 18 )
        */
        painter->translate( geometry().topLeft() );

        const qreal width( m_iconSize.width() );
        painter->scale( width/20, width/20 );
        painter->translate( 1, 1 );

        // render background
        const QColor backgroundColor( this->backgroundColor() );
        if( backgroundColor.isValid() )
        {
            painter->setPen( Qt::NoPen );
            painter->setBrush( backgroundColor.darker(120) );
            painter->drawEllipse( QRectF( 0, 0, 18, 18 ) );
            painter->setBrush( backgroundColor );
            painter->drawEllipse( QRectF( 1, 1, 16, 16 ) );
        }

        // render mark
        const QColor foregroundColor( this->foregroundColor() );
        if( foregroundColor.isValid() )
        {

            // setup painter
            QPen pen( foregroundColor );
            pen.setCapStyle( Qt::RoundCap );
            pen.setJoinStyle( Qt::MiterJoin );
            pen.setWidthF( PenWidth::Symbol*qMax((qreal)1.3, 20/width ) );

            painter->setPen( pen );
            painter->setBrush( foregroundColor );

            switch( type() )
            {

                case DecorationButtonType::Close:
                {
                    painter->drawLine( 5, 5, 13, 13 );
                    painter->drawLine( 13, 5, 5, 13 );
                    break;
                }

                case DecorationButtonType::Maximize:
                {
                    if( isChecked() )
                    {
                        painter->drawPolygon( QVector<QPointF>{
                            QPointF( 11, 4 ),
                            QPointF( 11, 7 ),
                            QPointF( 14, 7 )} );
                        painter->drawPolygon( QVector<QPointF>{
                            QPointF( 4, 11 ),
                            QPointF( 7, 11 ),
                            QPointF( 7, 14 )} );

                    } else {
                        painter->drawPolygon( QVector<QPointF>{
                            QPointF( 8, 5 ),
                            QPointF( 13, 5 ),
                            QPointF( 13, 10 )} );
                        painter->drawPolygon( QVector<QPointF>{
                            QPointF( 10, 13 ),
                            QPointF( 5, 13 ),
                            QPointF( 5, 8 )} );
                    }
                    break;
                }

                case DecorationButtonType::Minimize:
                {
                    painter->drawLine( 4, 9, 14, 9 );
                    break;
                }

                case DecorationButtonType::OnAllDesktops:
                {
                    painter->setPen( Qt::NoPen );
                    painter->setBrush( foregroundColor );

                    if( isChecked())
                    {

                        // outer ring
                        painter->drawEllipse( QRectF( 3, 3, 12, 12 ) );

                        // center dot
                        QColor backgroundColor( this->backgroundColor() );
                        auto d = qobject_cast<Decoration*>( decoration() );
                        if( !backgroundColor.isValid() && d ) backgroundColor = d->titleBarColor();

                        if( backgroundColor.isValid() )
                        {
                            painter->setBrush( backgroundColor );
                            painter->drawEllipse( QRectF( 8, 8, 2, 2 ) );
                        }

                    } else {

                        painter->drawPolygon( QVector<QPointF> {
                            QPointF( 6.5, 8.5 ),
                            QPointF( 12, 3 ),
                            QPointF( 15, 6 ),
                            QPointF( 9.5, 11.5 )} );

                        painter->setPen( pen );
                        painter->drawLine( QPointF( 5.5, 7.5 ), QPointF( 10.5, 12.5 ) );
                        painter->drawLine( QPointF( 12, 6 ), QPointF( 4.5, 13.5 ) );
                    }
                    break;
                }

                case DecorationButtonType::Shade:
                {

                    if (isChecked())
                    {

                        painter->drawLine( QPointF( 4, 5.5 ), QPointF( 14, 5.5 ) );
                        painter->drawPolyline( QVector<QPointF> {
                            QPointF( 4, 8 ),
                            QPointF( 9, 13 ),
                            QPointF( 14, 8 )} );

                    } else {

                        painter->drawLine( QPointF( 4, 5.5 ), QPointF( 14, 5.5 ) );
                        painter->drawPolyline(  QVector<QPointF> {
                            QPointF( 4, 13 ),
                            QPointF( 9, 8 ),
                            QPointF( 14, 13 ) });
                    }

                    break;

                }

                case DecorationButtonType::KeepBelow:
                {

                    painter->drawPolyline(  QVector<QPointF> {
                        QPointF( 4, 5 ),
                        QPointF( 9, 10 ),
                        QPointF( 14, 5 ) });

                    painter->drawPolyline(  QVector<QPointF> {
                        QPointF( 4, 9 ),
                        QPointF( 9, 14 ),
                        QPointF( 14, 9 ) });
                    break;

                }

                case DecorationButtonType::KeepAbove:
                {
                    painter->drawPolyline(  QVector<QPointF> {
                        QPointF( 4, 9 ),
                        QPointF( 9, 4 ),
                        QPointF( 14, 9 ) });

                    painter->drawPolyline(  QVector<QPointF> {
                        QPointF( 4, 13 ),
                        QPointF( 9, 8 ),
                        QPointF( 14, 13 ) });
                    break;
                }


                case DecorationButtonType::ApplicationMenu:
                {
                    painter->drawRect( QRectF( 3.5, 4.5, 11, 1 ) );
                    painter->drawRect( QRectF( 3.5, 8.5, 11, 1 ) );
                    painter->drawRect( QRectF( 3.5, 12.5, 11, 1 ) );
                    break;
                }

                case DecorationButtonType::ContextHelp:
                {
                    QPainterPath path;
                    path.moveTo( 5, 6 );
                    path.arcTo( QRectF( 5, 3.5, 8, 5 ), 180, -180 );
                    path.cubicTo( QPointF(12.5, 9.5), QPointF( 9, 7.5 ), QPointF( 9, 11.5 ) );
                    painter->drawPath( path );

                    painter->drawRect( QRectF( 9, 15, 0.5, 0.5 ) );

                    break;
                }

                default: break;

            }

        }

    }

    //__________________________________________________________________
    QColor Button::foregroundColor() const
    {
        auto d = qobject_cast<Decoration*>( decoration() );
        if( !d ) {

            return QColor();

        } else if( isPressed() ) {

            return QColor(colorSymbol);

        } else if( ( type() == DecorationButtonType::KeepBelow || type() == DecorationButtonType::KeepAbove || type() == DecorationButtonType::Shade ) && isChecked() ) {

            return d->titleBarColor();

        } else if( m_animation->state() == QAbstractAnimation::Running ) {

            auto c = d->client().toStrongRef();
            if ( !c->isActive() ) {
                QColor color(colorSymbol);
                color.setAlpha(255*m_opacity);
                return color;
            }

            QColor color;
            if( type() == DecorationButtonType::Close ) {
                color.setRgb(colorClose);
            } else if( type() == DecorationButtonType::Maximize ) {
                color.setRgb(colorMaximize);
            } else if( type() == DecorationButtonType::Minimize ) {
                color.setRgb(colorMinimize);
            } else{
                color.setRgb(colorOther);
            }
            return KColorUtils::mix( color, QColor(colorSymbol), m_opacity );

        } else if( isHovered() ) {

            return QColor(colorSymbol);

        } else {

            return backgroundColor();

        }

    }

    //__________________________________________________________________
    QColor Button::backgroundColor() const
    {
        auto d = qobject_cast<Decoration*>( decoration() );
        if( !d ) {

            return QColor();

        }

        auto c = d->client().toStrongRef();
        QColor redColor( c->color( ColorGroup::Warning, ColorRole::Foreground ) );

        if( isPressed() ) {

            QColor color;
            if( type() == DecorationButtonType::Close ) {
                color.setRgb(colorClose);
            } else if( type() == DecorationButtonType::Maximize ) {
                color.setRgb(colorMaximize);
            } else if( type() == DecorationButtonType::Minimize ) {
                color.setRgb(colorMinimize);
            } else{
                color.setRgb(colorOther);
            }
            return KColorUtils::mix( color, QColor(colorSymbol), 0.3 );

        } else if( ( type() == DecorationButtonType::KeepBelow || type() == DecorationButtonType::KeepAbove || type() == DecorationButtonType::Shade ) && isChecked() ) {

            return d->fontColor();

        } else if( !c->isActive() ) {

            QColor color;
            color.setRgb(colorInactive);
            return color;

        } else {

            QColor color;
            if( type() == DecorationButtonType::Close ) {
                color.setRgb(colorClose);
            } else if( type() == DecorationButtonType::Maximize ) {
                color.setRgb(colorMaximize);
            } else if( type() == DecorationButtonType::Minimize ) {
                color.setRgb(colorMinimize);
            } else{
                color.setRgb(colorOther);
            }
            return color;

        }

    }

    //________________________________________________________________
    void Button::reconfigure()
    {

        // animation
        auto d = qobject_cast<Decoration*>(decoration());
        if( d )  m_animation->setDuration( d->animationsDuration() );

    }

    //__________________________________________________________________
    void Button::updateAnimationState( bool hovered )
    {

        auto d = qobject_cast<Decoration*>(decoration());
        if( !(d && d->animationsDuration() > 0 ) ) return;

        m_animation->setDirection( hovered ? QAbstractAnimation::Forward : QAbstractAnimation::Backward );
        if( m_animation->state() != QAbstractAnimation::Running ) m_animation->start();

    }

} // namespace

/***************************************************************************
 *   Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_THUMBNAILLIST_H_
#define _KPDF_THUMBNAILLIST_H_

#include <qscrollview.h>
#include <qvaluevector.h>
#include <qvbox.h>
#include "core/observer.h"

class QTimer;
class KActionCollection;
class ThumbnailWidget;

/**
 * @short A scrollview that displays pages pixmaps previews (aka thumbnails).
 *
 * ...
 */
class ThumbnailList : public QScrollView, public KPDFDocumentObserver
{
Q_OBJECT
	public:
		ThumbnailList(QWidget *parent, KPDFDocument *document);

        // inherited: return thumbnails observer id
        uint observerId() const { return THUMBNAILS_ID; }
        // inherited: create thumbnails ( inherited as a DocumentObserver )
        void pageSetup( const QValueVector<KPDFPage*> & pages, bool documentChanged );
        // inherited: hilihght current thumbnail ( inherited as DocumentObserver )
        void pageSetCurrent( int pageNumber, const QRect & viewport );
        // inherited: tell if pixmap is hidden and can be unloaded
        bool canUnloadPixmap( int pageNumber );
        // inherited: redraw thumbnail ( inherited as DocumentObserver )
        void notifyPixmapChanged( int pageNumber );
        // inherited: request all visible pixmap (due to a global shange or so..)
        void notifyPixmapsCleared();

        // redraw visible widgets (useful for refreshing contents...)
        void updateWidgets();

	protected:
		// scroll up/down the view
		void keyPressEvent( QKeyEvent * e );

		// select a thumbnail by clicking on it
		void contentsMousePressEvent( QMouseEvent * );

		// resize thumbnails to fit the width
		void viewportResizeEvent( QResizeEvent * );

		// file drop related events (an url may be dropped even here)
		void dragEnterEvent( QDragEnterEvent* );
		void dropEvent( QDropEvent* );

	signals:
		void urlDropped( const KURL& );

	public slots:
		// make requests for generating pixmaps for visible thumbnails
		void slotRequestPixmaps( int newContentsX = -1, int newContentsY = -1 );

	private:
		void requestPixmaps( int delayMs = 0 );
		KPDFDocument *m_document;
		ThumbnailWidget *m_selected;
		QTimer *m_delayTimer;
		QValueVector<ThumbnailWidget *> m_thumbnails;
		QValueList<ThumbnailWidget *> m_visibleThumbnails;
		int m_vectorIndex;
};

/**
 * @short A vertical boxed container with zero size hint (for insertion on left toolbox)
 */
class ThumbnailsBox : public QVBox
{
	public:
		ThumbnailsBox( QWidget * parent ) : QVBox( parent ) {};
		QSize sizeHint() const { return QSize(); }
};

#endif

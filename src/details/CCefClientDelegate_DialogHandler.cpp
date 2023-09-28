#include "CCefClientDelegate.h"

#include <QDebug>
#include <QFileDialog>
#include <QSharedPointer>
#include <QThread>

#include "QCefViewPrivate.h"
#include "utils/CommonUtils.h"
#include "utils/ValueConvertor.h"

bool
CCefClientDelegate::onFileDialog(CefRefPtr<CefBrowser> browser,
                                 CefBrowserHost::FileDialogMode mode,
                                 const CefString& title,
                                 const CefString& default_file_path,
                                 const std::vector<CefString>& accept_filters,
                                 CefRefPtr<CefFileDialogCallback> callback)
{
  if (!pCefViewPrivate_)
    return false;
#if defined(Q_OS_WIN)
  return false;
#elif defined(Q_OS_LINUX)
  QFileDialog::FileMode fileMode = QFileDialog::AnyFile;
  QFileDialog::AcceptMode acceptMode = QFileDialog::AcceptOpen;
  switch (mode) {
    case FILE_DIALOG_OPEN:
      fileMode = QFileDialog::ExistingFile;
      break;
    case FILE_DIALOG_OPEN_MULTIPLE:
      fileMode = QFileDialog::ExistingFiles;
      break;
    case FILE_DIALOG_OPEN_FOLDER:
      fileMode = QFileDialog::Directory;
      break;
    case FILE_DIALOG_SAVE:
    default:
      acceptMode = QFileDialog::AcceptSave;
      break;
  }

  QStringList accepts;

  QString line;
  for (const CefString& accept : accept_filters) {
    line.append("*" + QString::fromStdString(accept.ToString()) + " ");
  }

  // TODO:Maybe translate to Chinese?
  accepts.append(line);
  accepts.append("All files (*)");

  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    pCefViewPrivate_->onFileDialog(acceptMode,
                                   fileMode,
                                   QString::fromStdString(title),
                                   QString::fromStdString(default_file_path),
                                   accepts,
                                   callback);
  });

  // TODO:Return the true or false as needed
  return true;
#endif
}

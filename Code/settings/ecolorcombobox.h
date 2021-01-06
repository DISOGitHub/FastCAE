// -*- Mode: c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2  -*-
//---------------------------------------------------------------------------
#ifndef ECOLORCOMBOBOX_H
#define ECOLORCOMBOBOX_H

#include <qcolor.h>
#include <qcombobox.h>
//#include "ISPostShareAPI.h"

/**
 * @brief ��ɫѡ�񸴺Ͽ�
 *
 * ��ɫѡ�񸴺Ͽ�Ϊ�û��ṩ��һ�ַ���ѡ��Ԥ������ɫ�ķ�����ͬʱҲ���Դ���
 * ɫѡ��Ի���ѡ��������ɫ��
 *
 * @note ���ڻ���QComboBox��clear���������麯��������޷����ء��������ɫ��
 *       ���򼤻�������ʹ����clear����������Ͽ��ڵ����ݣ������ɫ�Ի���
 *       ����Ҳ�ᱻ������������¼�����ɫ�Ի���������Ҫ������ݣ�����ʹ
 *       �ñ����ṩ�ķ���clearAllColors()��
 */

class EColorComboBox :public QComboBox
  {
  Q_OBJECT
  Q_PROPERTY(bool colorDialogEnabled READ colorDialogEnabled \
                                     WRITE enableColorDialog)
  public:
    /**
     * ���캯��������һ����ɫѡ�񸴺Ͽ����
     *
     * @param parent �����ڡ�
     */
    EColorComboBox(QWidget *parent=0);
    /**
     * ��ȡ��ǰѡ�����ɫ��
     *
     * @return ��ǰѡ�����ɫ��
     */
    QColor currentColor() const;
    /**
     * ��ȡ����indexָ������ɫ�����������Ч���򷵻���Ч����ɫ��
     *
     * @param index Ҫ��ȡ����ɫ��������
     * @return ��ȡ����ɫ��
     */
    QColor color(int index) const;
    /**
     * ����Ƿ�ʹ����ɫѡ��Ի���
     *
     * @return ���ʹ����ɫѡ��Ի����򷵻�true�����򷵻�false��
     */
    bool colorDialogEnabled() const;
    /**
     * ��ȡ�ʺ���ʾ���ؼ��ĳߴ硣
     *
     * @return �ʺ���ʾ���ؼ��ĳߴ硣
     */
    QSize sizeHint() const;
    /**
     * ���һ����ɫ�Ƿ��Ѿ��ڱ��ؼ����ˡ�
     *
     * @param color Ҫ������ɫ��
     * @return ����ڣ�����true�����򷵻�false��
     */
    bool hasColor(const QColor &color);
	void appendOtherColor();
  public slots:
    /**
     * ����б��ڵ�������ɫ����clear������ͬ���÷����������ɫ�Ի����
     */
    void clearAllColors();
    /**
     * ���б�ĩβ���һ����ɫ�����������ɫ�Ѿ����б����ˣ���ֱ�ӷ��ء�
     *
     * @param c    Ҫ��ӵ���ɫ��
     * @param name Ҫ��ӵ���ɫ��������
     */
    void appendColor(const QColor &c, const QString &name=QString());
    /**
     * ���б�ָ��λ�����һ����ɫ�����������ɫ�Ѿ����б����ˣ���ֱ�ӷ��ء�
     * ���Ҫ��ӵ�λ�ò����ڣ������б�ĩβ��ӡ�
     *
     * @param c     Ҫ��ӵ���ɫ��
     * @param name  Ҫ��ӵ���ɫ��������
     * @param index Ҫ�����ɫ��λ�á�
     */
    void insertColor(const QColor &c, const QString &name=QString(),
	             int index=-1);
    /**
     * �ڵ�ǰ�б��ĩβ���Ԥ�������ɫ��
     */
    void appendPredefinedColors();
	void appendBackgroundColors();
	
	void updateOtherColor(const QColor &color);
    /**
     * �����Ƿ�ʹ����ɫѡ��Ի������ʹ����ɫѡ��Ի��򣬻����б�������
     * ��һ��������ɫѡ��Ի�����
     *
     * @param enabled ���Ϊtrue��ʹ����ɫѡ��Ի��򣬷���ʹ�á�
     */
    void enableColorDialog(bool enabled=true);
    /**
     * ���õ�ǰѡ�����ɫ�����������ɫ�����б��ڣ���ֱ�ӷ��ء�
     *
     * @param color Ҫ����Ϊ��ǰѡ�����ɫ��
     */
    void setCurrentColor(const QColor &color);
  signals:
    /**
     * ��ĳ����ɫ��ѡ��ʱ��������źš�
     *
     * @param color ��ѡ�е���ɫ��
     */
    void activated(const QColor &color);
    /**
     * ���б�򿪲��Ҹ�����ʾĳһ����ɫʱ��������źš�
     *
     * @param color ��������ʾ����ɫ��
     */
    void highlighted(const QColor &color);
  private slots:
    void emitActivatedColor(int index);
    void emitHighlightedColor(int index);
  private:
    bool _colorDialogEnabled;
    QColor _lastActivated;
  };

#endif


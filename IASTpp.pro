#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T02:07:39
#
#-------------------------------------------------

QT       += core gui charts webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = IAST++
TEMPLATE = app

SOURCES += main.cpp\
    iast/bet_isotherm.cpp \
    iast/bisection_solver.cpp \
    iast/dsl_isotherm.cpp \
    iast/dslf_isotherm.cpp \
    iast/henry_isotherm.cpp \
    iast/iast_core.cpp \
    iast/iast_exception.cpp \
    iast/interpolator_isotherm.cpp \
    iast/isotherm_exception.cpp \
    iast/isotherm_factory.cpp \
    iast/isotherm_utility.cpp \
    iast/item_isotherm.cpp \
    iast/langmuir_isotherm.cpp \
    iast/lf_isotherm.cpp \
    iast/linear_interpolator.cpp \
    iast/minimizer_exception.cpp \
    iast/quadratic_isotherm.cpp \
    iast/simplex.cpp \
    iast/simplex_solver.cpp \
    iast/solver_exception.cpp \
    iast/solver_factory.cpp \
    gui/iast_plotter/iast_plotter_data.cpp \
    gui/iast_plotter/iast_plotter_option.cpp \
    gui/iast_plotter/iast_plotter_widget.cpp \
    gui/iast_plotter/plotter_axis_option.cpp \
    gui/iast_plotter/plotter_core.cpp \
    gui/iast_plotter/plotter_row_range.cpp \
    gui/iast_plotter/plotter_style_option.cpp \
    gui/iast_plotter/plotter_x_expression.cpp \
    gui/iast_plotter/plotter_y_expression.cpp \
    gui/muparser/muParser.cpp \
    gui/muparser/muParserBase.cpp \
    gui/muparser/muParserBytecode.cpp \
    gui/muparser/muParserCallback.cpp \
    gui/muparser/muParserDLL.cpp \
    gui/muparser/muParserError.cpp \
    gui/muparser/muParserInt.cpp \
    gui/muparser/muParserTest.cpp \
    gui/muparser/muParserTokenReader.cpp \
    gui/qcustomplot/qcustomplot.cpp \
    gui/core_calculator.cpp \
    gui/core_input_widget.cpp \
    gui/core_isotherm_table.cpp \
    gui/core_output_widget.cpp \
    gui/core_range_generator.cpp \
    gui/core_variable_chooser.cpp \
    gui/expression_parser.cpp \
    gui/home_widget.cpp \
    gui/iast_core_widget.cpp \
    gui/iast_gui.cpp \
    gui/iast_gui_exception.cpp \
    gui/iast_gui_logger.cpp \
    gui/iast_modeler_plot.cpp \
    gui/iast_modeler_widget.cpp \
    gui/isotherm_calculator.cpp \
    gui/isotherm_information.cpp \
    gui/modeler_option_dialog.cpp


HEADERS  += \
    iast/any.hpp \
    iast/any_impl.hpp \
    iast/bet_isotherm.hpp \
    iast/bisection_solver.hpp \
    iast/dsl_isotherm.hpp \
    iast/dslf_isotherm.hpp \
    iast/henry_isotherm.hpp \
    iast/iast.hpp \
    iast/iast_core.hpp \
    iast/iast_exception.hpp \
    iast/interpolator.hpp \
    iast/interpolator_isotherm.hpp \
    iast/isotherm.hpp \
    iast/isotherm_exception.hpp \
    iast/isotherm_factory.hpp \
    iast/isotherm_utility.hpp \
    iast/item_isotherm.hpp \
    iast/langmuir_isotherm.hpp \
    iast/lf_isotherm.hpp \
    iast/linear_interpolator.hpp \
    iast/minimizer.hpp \
    iast/minimizer_exception.hpp \
    iast/quadratic_isotherm.hpp \
    iast/simplex.hpp \
    iast/simplex_solver.hpp \
    iast/solver-backup.hpp \
    iast/solver.hpp \
    iast/solver_exception.hpp \
    iast/solver_factory.hpp \
    gui/iast_plotter/iast_plotter_data.h \
    gui/iast_plotter/iast_plotter_option.h \
    gui/iast_plotter/iast_plotter_widget.h \
    gui/iast_plotter/plotter_axis_option.h \
    gui/iast_plotter/plotter_core.h \
    gui/iast_plotter/plotter_row_range.h \
    gui/iast_plotter/plotter_style_option.h \
    gui/iast_plotter/plotter_x_expression.h \
    gui/iast_plotter/plotter_y_expression.h \
    gui/muparser/muParser.h \
    gui/muparser/muParserBase.h \
    gui/muparser/muParserBytecode.h \
    gui/muparser/muParserCallback.h \
    gui/muparser/muParserDef.h \
    gui/muparser/muParserDLL.h \
    gui/muparser/muParserError.h \
    gui/muparser/muParserFixes.h \
    gui/muparser/muParserInt.h \
    gui/muparser/muParserStack.h \
    gui/muparser/muParserTemplateMagic.h \
    gui/muparser/muParserTest.h \
    gui/muparser/muParserToken.h \
    gui/muparser/muParserTokenReader.h \
    gui/qcustomplot/qcustomplot.h \
    gui/core_calculator.h \
    gui/core_input_widget.h \
    gui/core_isotherm_table.h \
    gui/core_output_widget.h \
    gui/core_range_generator.h \
    gui/core_variable_chooser.h \
    gui/expression_parser.h \
    gui/home_widget.h \
    gui/iast_core_widget.h \
    gui/iast_gui.h \
    gui/iast_gui_exception.h \
    gui/iast_gui_logger.h \
    gui/iast_modeler_plot.h \
    gui/iast_modeler_widget.h \
    gui/isotherm_calculator.h \
    gui/isotherm_information.h \
    gui/modeler_option_dialog.h

RESOURCES += \
    resources.qrc \
    qdarkstyle/style.qrc

RC_ICONS += images/Logo4.ico

DISTFILES += \
    gui/qcustomplot/qcustomplot.qch

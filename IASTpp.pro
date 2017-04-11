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
        iast_gui.cpp \
    iast_modeler_widget.cpp \
#    iast/arma_solver.cpp \
#    iast/backtracking_solver.cpp \
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
    iast_modeler_plot.cpp \
    modeler_option_dialog.cpp \
    isotherm_information.cpp \
    iast_gui_exception.cpp \
    iast_core_widget.cpp \
    core_isotherm_table.cpp \
    core_input_widget.cpp \
    core_variable_chooser.cpp \
    core_range_generator.cpp \
    core_calculator.cpp \
    core_output_widget.cpp \
    muparser/muParser.cpp \
    muparser/muParserBase.cpp \
    muparser/muParserBytecode.cpp \
    muparser/muParserCallback.cpp \
    muparser/muParserDLL.cpp \
    muparser/muParserError.cpp \
    muparser/muParserInt.cpp \
    muparser/muParserTest.cpp \
    muparser/muParserTokenReader.cpp \
    expression_parser.cpp \
    iast_plotter/iast_plotter_widget.cpp \
    iast_plotter/iast_plotter_data.cpp \
    iast_plotter/iast_plotter_option.cpp \
    iast_plotter/plotter_axis_option.cpp \
    iast_plotter/plotter_style_option.cpp \
    iast_plotter/plotter_row_range.cpp \
    iast_plotter/plotter_x_expression.cpp \
    iast_plotter/plotter_y_expression.cpp \
    iast_plotter/plotter_core.cpp \
    qcustomplot/qcustomplot.cpp \
    isotherm_calculator.cpp \
    home_widget.cpp \
    iast_gui_logger.cpp

HEADERS  += iast_gui.h \
    iast_modeler_widget.h \
    iast/any.hpp \
    iast/any_impl.hpp \
#    iast/arma_solver.hpp \
#    iast/backtracking_solver.hpp \
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
#    iast/root_finder.hpp \
#    iast/root_finder_def.hpp \
#    iast/root_finder_impl.hpp \
    iast/simplex.hpp \
    iast/simplex_solver.hpp \
    iast/solver-backup.hpp \
    iast/solver.hpp \
    iast/solver_exception.hpp \
    iast/solver_factory.hpp \
    iast_modeler_plot.h \
    modeler_option_dialog.h \
    isotherm_information.h \
    iast_gui_exception.h \
    iast_core_widget.h \
    core_isotherm_table.h \
    core_input_widget.h \
    core_variable_chooser.h \
    core_range_generator.h \
    core_calculator.h \
    core_output_widget.h \
    muparser/muParser.h \
    muparser/muParserBase.h \
    muparser/muParserBytecode.h \
    muparser/muParserCallback.h \
    muparser/muParserDef.h \
    muparser/muParserDLL.h \
    muparser/muParserError.h \
    muparser/muParserFixes.h \
    muparser/muParserInt.h \
    muparser/muParserStack.h \
    muparser/muParserTemplateMagic.h \
    muparser/muParserTest.h \
    muparser/muParserToken.h \
    muparser/muParserTokenReader.h \
    expression_parser.h \
    iast_plotter/iast_plotter_widget.h \
    iast_plotter/iast_plotter_data.h \
    iast_plotter/iast_plotter_option.h \
    iast_plotter/plotter_axis_option.h \
    iast_plotter/plotter_style_option.h \
    iast_plotter/plotter_row_range.h \
    iast_plotter/plotter_x_expression.h \
    iast_plotter/plotter_y_expression.h \
    iast_plotter/plotter_core.h \
    qcustomplot/qcustomplot.h \
    isotherm_calculator.h \
    home_widget.h \
    iast_gui_logger.h

RESOURCES += \
    resources.qrc \
    qdarkstyle/style.qrc

RC_ICONS += images/Logo4.ico

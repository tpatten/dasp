#include "WdgtSuperpixelParameters.h"

WdgtSuperpixelParameters::WdgtSuperpixelParameters(const boost::shared_ptr<dasp::DaspTracker>& dasp_tracker, QWidget *parent)
    : QWidget(parent)
{
	dasp_tracker_ = dasp_tracker;

	ui.setupUi(this);

	ui.comboBoxSeedType->addItem("EquiDistant", dasp::SeedModes::EquiDistant);
	ui.comboBoxSeedType->addItem("DepthMipmap", dasp::SeedModes::DepthMipmap);
	ui.comboBoxSeedType->addItem("DepthBlueNoise", dasp::SeedModes::DepthBlueNoise);
	ui.comboBoxSeedType->addItem("Delta", dasp::SeedModes::Delta);
	ui.comboBoxSeedType->setCurrentIndex(1);

	ui.comboBoxPlotPointsColor->addItem("Color", dasp::plots::Color);
	ui.comboBoxPlotPointsColor->addItem("Depth", dasp::plots::Depth);
	ui.comboBoxPlotPointsColor->addItem("Valid", dasp::plots::Valid);
	ui.comboBoxPlotPointsColor->addItem("Gradient", dasp::plots::Gradient);
	ui.comboBoxPlotPointsColor->setCurrentIndex(0);

	ui.comboBoxPlotClusterColor->addItem("UniBlack", dasp::plots::UniBlack);
	ui.comboBoxPlotClusterColor->addItem("UniWhite", dasp::plots::UniWhite);
	ui.comboBoxPlotClusterColor->addItem("Color", dasp::plots::Color);
	ui.comboBoxPlotClusterColor->addItem("Depth", dasp::plots::Depth);
	ui.comboBoxPlotClusterColor->addItem("Gradient", dasp::plots::Gradient);
	ui.comboBoxPlotClusterColor->addItem("Thickness", dasp::plots::Thickness);
	ui.comboBoxPlotClusterColor->addItem("Circularity", dasp::plots::Circularity);
	ui.comboBoxPlotClusterColor->addItem("Eccentricity", dasp::plots::Eccentricity);
	ui.comboBoxPlotClusterColor->addItem("AreaQuotient", dasp::plots::AreaQuotient);
	ui.comboBoxPlotClusterColor->addItem("CoverageError", dasp::plots::CoverageError);
	ui.comboBoxPlotClusterColor->setCurrentIndex(2);

	ui.comboBoxPlotClusterMode->addItem("ClusterCenter", dasp::plots::ClusterCenter);
	ui.comboBoxPlotClusterMode->addItem("ClusterPoints", dasp::plots::ClusterPoints);
	ui.comboBoxPlotClusterMode->addItem("ClusterEllipses", dasp::plots::ClusterEllipses);
	ui.comboBoxPlotClusterMode->addItem("ClusterEllipsesFilled", dasp::plots::ClusterEllipsesFilled);
	ui.comboBoxPlotClusterMode->setCurrentIndex(1);

	QObject::connect(ui.checkBoxDaspRepairDepth, SIGNAL(stateChanged(int)), this, SLOT(ChangeDaspRepairDepth(int)));
	QObject::connect(ui.checkBoxDaspSmoothDepth, SIGNAL(stateChanged(int)), this, SLOT(ChangeDaspSmoothDepth(int)));
	QObject::connect(ui.comboBoxSeedType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSuperSeedType(int)));
	QObject::connect(ui.checkBoxGradientAdaptive, SIGNAL(stateChanged(int)), this, SLOT(ChangeSuperUseGradientDensity(int)));
	QObject::connect(ui.doubleSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelRadius(double)));
	QObject::connect(ui.spinBoxSuperCount, SIGNAL(valueChanged(int)), this, SLOT(ChangeSuperpixelCount(int)));
	QObject::connect(ui.spinBoxIterations, SIGNAL(valueChanged(int)), this, SLOT(ChangeSuperpixelIterations(int)));
	QObject::connect(ui.doubleSpinBoxWeightColor, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelWeightColor(double)));
	QObject::connect(ui.doubleSpinBoxWeightSpatial, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelWeightSpatial(double)));
	QObject::connect(ui.doubleSpinBoxWeightDepth, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelWeightDepth(double)));
	QObject::connect(ui.doubleSpinBoxWeightNormal, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelWeightNormal(double)));
	QObject::connect(ui.doubleSpinBoxCoverage, SIGNAL(valueChanged(double)), this, SLOT(ChangeSuperpixelCoverage(double)));
	QObject::connect(ui.checkBoxDaspConquerEnclaves, SIGNAL(stateChanged(int)), this, SLOT(ChangeSuperConquerEnclaves(int)));
	QObject::connect(ui.doubleSpinBoxDaspSegmentThreshold, SIGNAL(valueChanged(double)), this, SLOT(ChangeDaspSegmentThreshold(double)));


	QObject::connect(ui.pushButtonColorModelTrain, SIGNAL(clicked()), this, SLOT(OnColorModelTrain()));
	QObject::connect(ui.doubleSpinBoxColorSoftness, SIGNAL(valueChanged(double)), this, SLOT(ChangeColorModelSigmaScale(double)));

	QObject::connect(ui.checkBoxPlotPoints, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotPoints(int)));
	QObject::connect(ui.comboBoxPlotPointsColor, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangePlotPointsColor(int)));
	QObject::connect(ui.checkBoxPlotClusters, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotClusters(int)));
	QObject::connect(ui.comboBoxPlotClusterMode, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangePlotClusterMode(int)));
	QObject::connect(ui.comboBoxPlotClusterColor, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangePlotClusterColor(int)));
	QObject::connect(ui.checkBoxPlotBorders, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotBorders(int)));
	QObject::connect(ui.checkBoxPlotGraph, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotGraph(int)));
	QObject::connect(ui.checkBoxPlotDensity, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotDensity(int)));
	QObject::connect(ui.checkBoxPlotSegments, SIGNAL(stateChanged(int)), this, SLOT(ChangePlotSegments(int)));

	dasp_tracker_->dasp_params->is_repair_depth = ui.checkBoxDaspRepairDepth->isChecked();
	dasp_tracker_->dasp_params->is_smooth_depth = ui.checkBoxDaspSmoothDepth->isChecked();
	dasp_tracker_->dasp_params->gradient_adaptive_density = ui.checkBoxGradientAdaptive->isChecked();
	dasp_tracker_->dasp_params->base_radius = 0.001f * ui.doubleSpinBoxRadius->value();
	dasp_tracker_->dasp_params->count = ui.spinBoxSuperCount->value();
	dasp_tracker_->dasp_params->iterations = ui.spinBoxIterations->value();
	dasp_tracker_->dasp_params->weight_color = ui.doubleSpinBoxWeightColor->value();
	dasp_tracker_->dasp_params->weight_spatial = ui.doubleSpinBoxWeightSpatial->value();
	dasp_tracker_->dasp_params->weight_depth = ui.doubleSpinBoxWeightDepth->value();
	dasp_tracker_->dasp_params->weight_normal = ui.doubleSpinBoxWeightNormal->value();
	dasp_tracker_->dasp_params->coverage = ui.doubleSpinBoxCoverage->value();
	dasp_tracker_->dasp_params->is_conquer_enclaves = ui.checkBoxDaspConquerEnclaves->isChecked();
	dasp_tracker_->dasp_params->segment_threshold = ui.doubleSpinBoxDaspSegmentThreshold->value();

	dasp_tracker_->show_points_ = ui.checkBoxPlotPoints->isChecked();
	dasp_tracker_->point_color_mode_ = (dasp::plots::ColorMode)(ui.comboBoxPlotPointsColor->itemData(ui.comboBoxPlotPointsColor->currentIndex()).toInt());
	dasp_tracker_->show_clusters_ = ui.checkBoxPlotClusters->isChecked();;
	dasp_tracker_->cluster_mode_ = (dasp::plots::ClusterMode)(ui.comboBoxPlotClusterMode->itemData(ui.comboBoxPlotClusterMode->currentIndex()).toInt());
	dasp_tracker_->cluster_color_mode_ = (dasp::plots::ColorMode)(ui.comboBoxPlotClusterColor->itemData(ui.comboBoxPlotClusterColor->currentIndex()).toInt());
	dasp_tracker_->show_cluster_borders_ = ui.checkBoxPlotBorders->isChecked();
	dasp_tracker_->show_graph_ = ui.checkBoxPlotGraph->isChecked();
	dasp_tracker_->plot_density_ = ui.checkBoxPlotDensity->isChecked();
	dasp_tracker_->plot_segments_ = ui.checkBoxPlotSegments->isChecked();

}

WdgtSuperpixelParameters::~WdgtSuperpixelParameters()
{

}

void WdgtSuperpixelParameters::ChangeDaspRepairDepth(int state)
{
	dasp_tracker_->dasp_params->is_repair_depth = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeDaspSmoothDepth(int state)
{
	dasp_tracker_->dasp_params->is_smooth_depth = state;
	*reload = true;
}

void WdgtSuperpixelParameters::OnSuperSeedType(int selection)
{
	dasp_tracker_->dasp_params->seed_mode = (dasp::SeedMode)(ui.comboBoxSeedType->itemData(selection).toInt());
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperUseGradientDensity(int state)
{
	dasp_tracker_->dasp_params->gradient_adaptive_density = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelRadius(double val)
{
	dasp_tracker_->dasp_params->base_radius = 0.001f * val;
	dasp_tracker_->dasp_params->count = 0;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelCount(int val)
{
	dasp_tracker_->dasp_params->count = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelIterations(int val)
{
	dasp_tracker_->dasp_params->iterations = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelWeightColor(double val)
{
	dasp_tracker_->dasp_params->weight_color = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelWeightSpatial(double val)
{
	dasp_tracker_->dasp_params->weight_spatial = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelWeightDepth(double val)
{
	dasp_tracker_->dasp_params->weight_depth = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelWeightNormal(double val)
{
	dasp_tracker_->dasp_params->weight_normal = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperConquerEnclaves(int val)
{
	dasp_tracker_->dasp_params->is_conquer_enclaves = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeSuperpixelCoverage(double val)
{
	dasp_tracker_->dasp_params->coverage = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeDaspSegmentThreshold(double val)
{
	dasp_tracker_->dasp_params->segment_threshold = val;
	*reload = true;
}

void WdgtSuperpixelParameters::OnColorModelTrain()
{
	dasp_tracker_->training_ = true;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangeColorModelSigmaScale(double val)
{
	dasp_tracker_->color_model_sigma_scale_ = val;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotPoints(int state)
{
	dasp_tracker_->show_points_ = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotPointsColor(int selection)
{
	dasp_tracker_->point_color_mode_ = (dasp::plots::ColorMode)(ui.comboBoxPlotPointsColor->itemData(selection).toInt());
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotClusters(int state)
{
	dasp_tracker_->show_clusters_ = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotClusterMode(int selection)
{
	dasp_tracker_->cluster_mode_ = (dasp::plots::ClusterMode)(ui.comboBoxPlotClusterMode->itemData(selection).toInt());
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotClusterColor(int selection)
{
	dasp_tracker_->cluster_color_mode_ = (dasp::plots::ColorMode)(ui.comboBoxPlotClusterColor->itemData(selection).toInt());
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotBorders(int state)
{
	dasp_tracker_->show_cluster_borders_ = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotGraph(int state)
{
	dasp_tracker_->show_graph_ = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotDensity(int state)
{
	dasp_tracker_->plot_density_ = state;
	*reload = true;
}

void WdgtSuperpixelParameters::ChangePlotSegments(int state)
{
	dasp_tracker_->plot_segments_ = state;
	*reload = true;
}

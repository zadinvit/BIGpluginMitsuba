#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../include/MIFbtf.hpp"
using namespace mif;

namespace MIFlibTest
{
    TEST_CLASS(MIFbtfUnitTest)
    {
    public:

        TEST_METHOD(TestEmptyConstructor)
        {
            {
                // test that empty constructor does nothing
                MIFbtf mif1;

// todo                Assert::AreEqual(std::string("mif"), mif.getRoot().getName());


                //using namespace mif;
                //using namespace mif::directions;
                //using namespace mif::mipmap;
                //MIFbtf mif("data.mif");
                //mif.addImage(index, image);
                //mif.addBtfElement("btf0", "uniform");
                //Directions directions;
                //directions.type = Directions::Type::list;
                //directions.name = "VU";
                //directions.listRecords.push_back({ Direction(Type::source, 0, 0), Direction(Type::sensor, 15, 0) });
                //directions.listRecords.push_back({ Direction(Type::source, 15, 0), Direction(Type::sensor, 15, 0) });
                //directions.listRecords.push_back({ Direction(Type::source, 15, 30), Direction(Type::sensor, 15, 0) });
                //directions.listRecords.push_back({ Direction(Type::source, 15, 60), Direction(Type::sensor, 30, 180) });
                //directions.attributes = { { "stepTheta", "15" },{ "stepPhi","30" } };
                //mif.addBtfDirectionsReference("btf0", "directions0", directions);
                //mif.addDirections("directions0", directions);
                //Mipmap mipmap;
                //mipmap.type = Mipmap::Type::isotropic;
                //mipmap.isotropic = { Item(0, 0, 100, 100), Item(100, 0, 50, 50), Item(100, 50, 25, 25), Item(100, 75, 12, 12), Item(100, 87, 6, 6), Item(100, 93, 3, 3), Item(100, 96, 2, 2), Item(100, 98, 1, 1) };
                //mif.addBtfMipmap("btf0", mipmap);
                //mif.saveXML();

                //{
                //    using namespace mif;
                //    using namespace mif::directions;
                //    using namespace mif::mipmap;
                //    MIFbtf mif("data.mif", true);
                //    mif.getBtfName("btf0");
                //    Directions directions = mif.getBtfDirections("btf0");
                //    mif.getBtfMipmap("btf0");
                //    MIFbtf::MIFcacheWhole cache = mif.getMIFcacheWhole("btf0");
                //    cache.getPixel(index, row, col);
                //}

            }
        }

        TEST_METHOD(TestSaveAndLoad)
        {
            {
                // test that empty constructor does nothing and file can be opened later
                MIFbtf mif;
                // add material
                mif.setMaterial(ElementMaterial("mat0", "sampleA", "companyX", "very nice material taken from company XYZ"));
                // add device
                ElementDevice device("dev0", "goniometer", "UTIA goniometer", "001", "text");
                // todo:
                //mif.addDeviceSource("dev0", "source0", "led");
                //mif.addDeviceSource("dev0", "source1", "led");
                //Element color_matrix("color-matrix", "1.038430, 0.488373, 0.573212, 0.054427, 2.096516, 0.099773, -0.035877, -2.339362, 4.795482");
                //mif.addDeviceSensor("dev0", "sensor0", "camera", {}, { color_matrix });
                //mif.addDeviceSensor("dev0", "sensor1", "camera");
                mif.setDevice(device);
                // add area
                ElementArea area("area0", 300, 400, 112, 120, 700, 800);
                area.setDPI(552.1739);
                area.setPixelsPerMillimeter(21.7391);
                area.setPixelSize(46.0, "um");
                mif.setArea(area);
                // add colors
                ElementColors colors("colors0");
                colors.setObserver(2);
                colors.setWhitePoint(Attributes({ { "X", "95.047" },{ "Y", "100.0" },{ "Z", "108.883" } }));
                colors.setColorChannel(0, "500nm");
                colors.setColorChannel(1, "520nm");
                colors.setColorChannel(2, "540nm");
                colors.setColorChannel(3, "X");
                mif.setColors(colors);
                // add btf
                std::vector<ElementDirectionsItem> items;
                items.push_back(ElementDirectionsItem(0, { ElementDirection(0, DirectionType::source, 0, 0), ElementDirection(1, DirectionType::sensor, 15.0, 0) }));
                items.push_back(ElementDirectionsItem(1, { ElementDirection(0, DirectionType::source, 15.0, 0), ElementDirection(1, DirectionType::sensor, 15.0, 0) }));
                items.push_back(ElementDirectionsItem(2, { ElementDirection(0, DirectionType::source, 15.0, 30.0), ElementDirection(1, DirectionType::sensor, 15.0, 0) }));
                items.push_back(ElementDirectionsItem(3, { ElementDirection(0, DirectionType::source, 15.0, 60.0), ElementDirection(1, DirectionType::sensor, 30.0, 180.0) }));
                ElementDirections directions("directions0", items);
                directions.setNameUniform(15.0, 30.0);
                ElementBtf btf("btf0", "Uniform measurement with step theta 15 degrees and step phi 30 degrees", "mat0", "dev0", "area0", "colors0");
                btf.setDirections(directions);
                btf.setIndexMap(ElementIndexMap({ 10, 7, 11, 2 }));
                btf.setMipmap(ElementMipmap(std::vector<ElementMipmapItem>({ ElementMipmapItem(0, 0, 0, 100, 100), ElementMipmapItem(1, 100, 0, 50, 50), ElementMipmapItem(2, 100, 50, 25, 25), ElementMipmapItem(3, 100, 75, 12, 12), ElementMipmapItem(4, 100, 87, 6, 6), ElementMipmapItem(5, 100, 93, 3, 3), ElementMipmapItem(6, 100, 96, 2, 2), ElementMipmapItem(7, 100, 98, 1, 1) })));
                mif.setBtf(btf);
                mif.setDirections(directions);
                // add image
                ElementTimestamps timestamps;
                timestamps.setTimestamp("time1", "2021-03-26T14:26:30");
                timestamps.setTimestamp("start", "2021-03-26T14:25:58");
                ElementDirectionsItem directionsItem(0, { ElementDirection(0, DirectionType::source, 15.0, 30.0, "deg", "source0"), ElementDirection(1, DirectionType::source, 45.0, 60.0, "deg", "source1"), ElementDirection(2, DirectionType::sensor, 75.0, 180.0, "deg", "sensor0") });
                ElementImage image(0, "mat0", "dev0", "area0", "colors0");
                image.setTimestamps(timestamps);
                image.setDirectionsItem(directionsItem);
                mif.setImageElement(image);

                // save
                mif.saveXMLtoSeparateFile("XML_Test3.xml");
            }
            {
                MIFbtf mif;
                // load
                mif.loadXMLfromSeparateFile("XML_Test3.xml");
                // check material
                Assert::AreEqual(size_t(1), mif.getMaterialIDs().size());
                Assert::AreEqual(std::string("mat0"), mif.getMaterialIDs().front());
                auto material = mif.getMaterial("mat0");
                Assert::AreEqual(std::string("sampleA"), material.getName());
                Assert::AreEqual(std::string("companyX"), material.getReference());
                Assert::AreEqual(std::string("very nice material taken from company XYZ"), material.getDescription());
                // check device
                Assert::AreEqual(size_t(1), mif.getDeviceIDs().size());
                Assert::AreEqual(std::string("dev0"), mif.getDeviceIDs().front());
                auto device = mif.getDevice("dev0");
                Assert::AreEqual(std::string("goniometer"), device.getType());
                Assert::AreEqual(std::string("UTIA goniometer"), device.getName());
                Assert::AreEqual(std::string("001"), device.getSerial());
                Assert::AreEqual(std::string("text"), device.getDescription());
                // todo:
                //Assert::AreEqual(size_t(2), mif.getDeviceSourcesIDs("dev0").size());
                //Assert::AreEqual(std::string("source0"), mif.getDeviceSourcesIDs("dev0").at(0));
                //Assert::AreEqual(std::string("source1"), mif.getDeviceSourcesIDs("dev0").at(1));
                // check area
                Assert::AreEqual(size_t(1), mif.getAreaIDs().size());
                Assert::AreEqual(std::string("area0"), mif.getAreaIDs().front());
                auto area = mif.getArea("area0");
                Assert::AreEqual(300.0, area.getAreaTop());
                Assert::AreEqual(400.0, area.getAreaLeft());
                Assert::AreEqual(112.0, area.getAreaHeight());
                Assert::AreEqual(120.0, area.getAreaWidth());
                Assert::AreEqual(700.0, area.getAreaWholeHeight());
                Assert::AreEqual(800.0, area.getAreaWholeWidth());
                Assert::AreEqual(552.1739, area.getDPI());
                Assert::AreEqual(21.7391, area.getPixelsPerMillimeter());
                Assert::AreEqual(46.0, area.getPixelSize());
                Assert::AreEqual(std::string("um"), area.getPixelSizeUnit());
                Assert::AreEqual(std::string("px"), area.getAreaUnits());
                // check colors
                Assert::AreEqual(size_t(1), mif.getColorsIDs().size());
                Assert::AreEqual(std::string("colors0"), mif.getColorsIDs().front());
                auto colors = mif.getColors("colors0");
                Assert::AreEqual(std::string("XYZ"), colors.getColorSpace());
                Assert::AreEqual(std::string("D65"), colors.getIlluminant());
                Assert::AreEqual(2.0, colors.getObserver());
                Assert::AreEqual(std::string("deg"), colors.getObserverUnit());
                auto whitePoint = colors.getWhitePoint();
                Assert::AreEqual(true, whitePoint.isAttribute("X"));
                Assert::AreEqual(95.047, std::stod(whitePoint.getAttribute("X")));
                Assert::AreEqual(true, whitePoint.isAttribute("Y"));
                Assert::AreEqual(100.0, std::stod(whitePoint.getAttribute("Y")));
                Assert::AreEqual(true, whitePoint.isAttribute("Z"));
                Assert::AreEqual(108.883, std::stod(whitePoint.getAttribute("Z")));
                Assert::AreEqual(size_t(4), colors.getColorChannelsCount());
                Assert::AreEqual(std::string("500nm"), colors.getColorChannel(0));
                Assert::AreEqual(std::string("520nm"), colors.getColorChannel(1));
                Assert::AreEqual(std::string("540nm"), colors.getColorChannel(2));
                Assert::AreEqual(std::string("X"), colors.getColorChannel(3));
                {
                    // check btf
                    Assert::AreEqual(size_t(1), mif.getBtfIDs().size());
                    Assert::AreEqual(std::string("btf0"), mif.getBtfIDs().front());
                    auto btf = mif.getBtf("btf0");
                    Assert::AreEqual(std::string("Uniform measurement with step theta 15 degrees and step phi 30 degrees"), btf.getName());
                    Assert::AreEqual(std::string("mat0"), btf.getMaterialId());
                    Assert::AreEqual(std::string("dev0"), btf.getDeviceId());
                    Assert::AreEqual(std::string("area0"), btf.getAreaId());
                    Assert::AreEqual(std::string("colors0"), btf.getColorsId());
                    Assert::AreEqual(false, btf.isDirections());
                    Assert::AreEqual(true, btf.isDirectionsReference());
                    auto directionsInfo = btf.getDirectionsInfo();
                    Assert::AreEqual(size_t(4), directionsInfo.getDirectionsCount());    
                    Assert::AreEqual(15.0, directionsInfo.getStepTheta());
                    Assert::AreEqual(30.0, directionsInfo.getStepPhi());
                    Assert::AreEqual(true, btf.isIndexMap());
                    auto map = btf.getIndexMap();
                    Assert::AreEqual(size_t(4), map.getIndices().size());
                    Assert::AreEqual(size_t(11), map.getIndices().at(2));
                    Assert::AreEqual(false, btf.isIndexOffset());
                    Assert::AreEqual(size_t(0), btf.getIndexOffset());
                    auto map2 = btf.getImageIndices();
                    Assert::AreEqual(size_t(4), map2.size());
                    Assert::AreEqual(size_t(11), map2.at(2));
                    Assert::AreEqual(true, btf.isMipmap());
                    auto mipmap = btf.getMipmap();
                    Assert::AreEqual(true, mipmap.isIsotropic());
                    Assert::AreEqual(size_t(8), mipmap.getIsotropicItems().size());
                    Assert::AreEqual(size_t(100), mipmap.getIsotropicItems()[6].getX());
                    Assert::AreEqual(size_t(96), mipmap.getIsotropicItems()[6].getY());
                    Assert::AreEqual(size_t(2), mipmap.getIsotropicItems()[6].getCols());
                    Assert::AreEqual(size_t(2), mipmap.getIsotropicItems()[6].getRows());

                    auto directions = mif.getDirections(directionsInfo.getAttribute("reference"));
                    {
                        auto items = directions.getDirectionsItem(0);
                        Assert::AreEqual(std::string("source"), directionTypeToString(items.getDirection(0).getType()));
                        Assert::AreEqual(0.0, items.getDirection(0).getTheta());
                        Assert::AreEqual(0.0, items.getDirection(0).getPhi());
                        Assert::AreEqual(std::string("sensor"), directionTypeToString(items.getDirection(1).getType()));
                        Assert::AreEqual(15.0, items.getDirection(1).getTheta());
                        Assert::AreEqual(0.0, items.getDirection(1).getPhi());
                    }
                    //{
                    //    auto items = directions.listRecords.at(1);
                    //    Assert::AreEqual(std::string("source"), directionTypeToString(items.at(0).type));
                    //    Assert::AreEqual(15.0, items.at(0).theta);
                    //    Assert::AreEqual(0.0, items.at(0).phi);
                    //    Assert::AreEqual(std::string("sensor"), directionTypeToString(items.at(1).type));
                    //    Assert::AreEqual(15.0, items.at(1).theta);
                    //    Assert::AreEqual(0.0, items.at(1).phi);
                    //}
                    //{
                    //    auto items = directions.listRecords.at(2);
                    //    Assert::AreEqual(std::string("source"), directionTypeToString(items.at(0).type));
                    //    Assert::AreEqual(15.0, items.at(0).theta);
                    //    Assert::AreEqual(30.0, items.at(0).phi);
                    //    Assert::AreEqual(std::string("sensor"), directionTypeToString(items.at(1).type));
                    //    Assert::AreEqual(15.0, items.at(1).theta);
                    //    Assert::AreEqual(0.0, items.at(1).phi);
                    //}
                    //{
                    //    auto items = directions.listRecords.at(3);
                    //    Assert::AreEqual(std::string("source"), directionTypeToString(items.at(0).type));
                    //    Assert::AreEqual(15.0, items.at(0).theta);
                    //    Assert::AreEqual(60.0, items.at(0).phi);
                    //    Assert::AreEqual(std::string("sensor"), directionTypeToString(items.at(1).type));
                    //    Assert::AreEqual(30.0, items.at(1).theta);
                    //    Assert::AreEqual(180.0, items.at(1).phi);
                    //}
                }
                // check image
                Assert::AreEqual(size_t(1), mif.getImageElementsIndices().size());
                Assert::AreEqual(true, mif.isImageElement(0));
                Assert::AreEqual(size_t(0), mif.getImageElementsIndices().front());
                auto image = mif.getImageElement(0);
                Assert::AreEqual(std::string("mat0"), image.getMaterialId());
                Assert::AreEqual(std::string("dev0"), image.getDeviceId());
                Assert::AreEqual(std::string("area0"), image.getAreaId());
                Assert::AreEqual(std::string("colors0"), image.getColorsId());
                auto timestamps = image.getTimestamps();;
                Assert::AreEqual(size_t(2), timestamps.getTimestampsCount());
                auto labels = timestamps.getTimestampsLabels();
                Assert::AreEqual(std::string("time1"), labels[0]);
                Assert::AreEqual(std::string("2021-03-26T14:26:30"), timestamps.getTimestamp(labels[0]));
                Assert::AreEqual(std::string("start"), labels[1]);
                Assert::AreEqual(std::string("2021-03-26T14:25:58"), timestamps.getTimestamp(labels[1]));

                auto directionsItem = image.getDirectionsItem();
                Assert::AreEqual(std::string("source"), directionTypeToString(directionsItem.getDirection(0).getType()));
                Assert::AreEqual(15.0, directionsItem.getDirection(0).getTheta());
                Assert::AreEqual(30.0, directionsItem.getDirection(0).getPhi());
                Assert::AreEqual(std::string("source0"), directionsItem.getDirection(0).getReference());
                Assert::AreEqual(std::string("source"), directionTypeToString(directionsItem.getDirection(1).getType()));
                Assert::AreEqual(45.0, directionsItem.getDirection(1).getTheta());
                Assert::AreEqual(60.0, directionsItem.getDirection(1).getPhi());
                Assert::AreEqual(std::string("source1"), directionsItem.getDirection(1).getReference());
                Assert::AreEqual(std::string("sensor"), directionTypeToString(directionsItem.getDirection(2).getType()));
                Assert::AreEqual(75.0, directionsItem.getDirection(2).getTheta());
                Assert::AreEqual(180.0, directionsItem.getDirection(2).getPhi());
                Assert::AreEqual(std::string("sensor0"), directionsItem.getDirection(2).getReference());
            }
        }




        TEST_METHOD(TestMixedElement)
        {
            {
                // add mixed element
                remove("test_mixed.mif");
                MIFbtf mif("test_mixed.mif");
                Element e("mixed", "main-text", Attributes({ { "id", "mixed0" } }));
                e.pushChildElement(Element("child", "child-text"));
                mif.pushChildElement(e);
                mif.saveXML();
                mif.saveXMLtoSeparateFile("test_mixed.xml");
            }
            {
                // check mixed element
                MIFbtf mif("test_mixed.mif");
                const Element e = mif.getChildElement("mixed");
                Assert::AreEqual(std::string("main-text"), e.getElementText());
                Assert::AreEqual(std::string("mixed0"), e.getAttribute("id"));
                Assert::AreEqual(true, e.isChildElement("child"));
                Assert::AreEqual(std::string("child-text"), e.getChildElement("child").getElementText());
                mif.saveXMLtoSeparateFile("test_mixed_2.xml");
            }
        }

        TEST_METHOD(TestSaveAndLoad2)
        {
            {
                remove("test1.mif");

                MIFbtf mif("test1.mif");
                ElementMaterial material("mat0", "sampleA", "companyX", "very nice material taken from company X");
                mif.setMaterial(material);
                //ElementBtf btf("btf0", "uniform");
                //mif.setBtf(btf);
                mif.saveXML();
                mif.saveXMLtoSeparateFile("test1a.xml");
            }
            {
                MIFbtf mif("test1.mif", true);
                mif.saveXMLtoSeparateFile("test1.xml");
            }
        }



        // TODO: all other tests

    };
}
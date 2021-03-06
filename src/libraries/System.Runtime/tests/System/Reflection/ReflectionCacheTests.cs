// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using Xunit;

namespace System.Reflection.Tests
{
    public class ReflectionCacheTests
    {
        [Fact]
        public void GetMethod_MultipleCalls_SameObjects()
        {
            MethodInfo mi1 = typeof(ReflectionCacheTests).GetMethod(nameof(GetMethod_MultipleCalls_SameObjects));
            Assert.NotNull(mi1);

            MethodInfo mi2 = typeof(ReflectionCacheTests).GetMethod(nameof(GetMethod_MultipleCalls_SameObjects));
            Assert.NotNull(mi2);

            Assert.Same(mi1, mi2);
        }

        [ActiveIssue("https://github.com/dotnet/runtime/issues/50978", TestRuntimes.Mono)]
        [Fact]
        public void GetMethod_MultipleCalls_ClearCache_DifferentObjects()
        {
            Type updateHandler = typeof(Type).Assembly.GetType("System.Reflection.Metadata.RuntimeTypeMetadataUpdateHandler", throwOnError: true, ignoreCase: false);
            MethodInfo beforeUpdate = updateHandler.GetMethod("BeforeUpdate", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static, new[] { typeof(Type) });
            Assert.NotNull(beforeUpdate);

            MethodInfo mi1 = typeof(ReflectionCacheTests).GetMethod(nameof(GetMethod_MultipleCalls_ClearCache_DifferentObjects));
            Assert.NotNull(mi1);
            Assert.Equal(nameof(GetMethod_MultipleCalls_ClearCache_DifferentObjects), mi1.Name);

            beforeUpdate.Invoke(null, new object[] { typeof(ReflectionCacheTests) });

            MethodInfo mi2 = typeof(ReflectionCacheTests).GetMethod(nameof(GetMethod_MultipleCalls_ClearCache_DifferentObjects));
            Assert.NotNull(mi2);
            Assert.Equal(nameof(GetMethod_MultipleCalls_ClearCache_DifferentObjects), mi2.Name);

            Assert.NotSame(mi1, mi2);
        }
    }
}
